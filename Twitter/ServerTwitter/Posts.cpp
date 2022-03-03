#include "Posts.h"
#include "UtilityFunctions.h"
#include <chrono>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <queue>
#include <utility>
#include <algorithm>
#include <random>
bool Posts::CreatePost(const std::string& username, const std::string& postedDate, const std::string& text, const std::string& RetweetID)
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][CreatePost] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	std::vector<std::string> insertColumns{ "Username","PostedDate","Text" };
	std::vector<std::string> insertValues{ username,postedDate,text };
	std::string query = "SELECT ID_Post FROM Posts WHERE Username = '" + username + "' AND PostedDate = '" + postedDate +
		"' AND Text = '" + text + "'";
	if (RetweetID != "0")
	{
		if (!m_db->CheckExists("Posts", "ID_Post = '" + RetweetID + "'"))
		{
			m_logger->WriteLog("[Server][Posts][CreatePost] Post with ID = " + RetweetID + " doesn't exist in DB", Logger::Level::ERROR);
			return false;
		}
		insertColumns.push_back("ID_Retweet");
		insertValues.push_back(RetweetID);
		query = query + " AND ID_Retweet = '" + RetweetID + "'";
	}
	m_db->InsertIntoDB("Posts", insertColumns, insertValues);
	std::vector<std::string> postsId = m_db->GetTemplate<std::vector<std::string>>(query);
	AddToInvertedIndex(postsId, text);
	if (RetweetID == "-1")
	{
		m_logger->WriteLog("[Server][Posts][CreatePost] Created post from user = " + username + " , and text = " + text, Logger::Level::INFO);
	}
	else
	{
		m_logger->WriteLog("[Server][Posts][CreatePost] Created retweet from user = " + username + " to post with ID =" + RetweetID +
			", with text = " + text, Logger::Level::INFO);
	}
	return true;
}

std::string Posts::GetPost(const std::string& postID) const
{
	std::string result = postID;
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GetPost] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		result.clear();
		return result;
	}
	const std::vector<std::string> dbOutput = m_db->GetTemplate<std::vector<std::string>>("SELECT Username,Likes,PostedDate,Text FROM Posts WHERE ID_Post = '" + postID + "'");
	for (std::vector<std::string>::const_iterator iter = dbOutput.cbegin(); iter != dbOutput.cend(); iter++)
	{
		result = result + " " + *iter;
	}
	std::string retweetID = m_db->GetTemplate<std::string>("SELECT ID_Retweet FROM Posts WHERE ID_Post = '" + postID + "'");
	if (retweetID != "")
	{
		result = result + " " + retweetID;
	}
	else
	{
		result = result + " 0";
	}
	return result;
}

Posts::Posts()
{
	InitializeInvertedIndex();
}

bool Posts::ChangeLike(const std::string& postID, const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][ChangeLike] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][ChangeLike] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (!m_db->CheckExists("LikesDislikes", "ID_Post = '" + postID + "' AND Username = '" + username + "'"))
	{
		std::vector<std::string> insertColumns{ "ID_Post","Username" };
		std::vector<std::string> insertValues{ postID,username };
		m_db->InsertIntoDB("LikesDislikes", insertColumns, insertValues);
		m_logger->WriteLog("[Server][Posts][ChangeLike] User " + username + " likes post with ID = " + postID, Logger::Level::ERROR);
	}
	else
	{
		m_db->DeleteFromDB("LikesDislikes", "ID_Post = '" + postID + "' AND Username='" + username + "'");
		m_logger->WriteLog("[Server][Posts][ChangeLike] User " + username + " doesn't like the post with ID = " + postID + " anymore", Logger::Level::ERROR);
	}
	return true;
}

bool Posts::CheckLiked(const std::string& postID, const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][CheckLiked] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][CheckLiked] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	bool result = m_db->CheckExists("LikesDislikes", "ID_Post = '" + postID + "' AND Username = '" + username + "'");
	m_logger->WriteLog("[Server][Posts][CheckLiked] Checked if " + username + " likes post with ID = " + postID + ". Result: " + std::to_string(result), Logger::Level::ERROR);
	if (result)
	{
		return true;
	}
	return false;
}

int Posts::CountLikes(const std::string& postID) const
{
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][CountLikes] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return 0;
	}
	std::string result = m_db->GetTemplate<std::string>("SELECT Likes FROM Posts WHERE ID_Post = '" + postID + "'");
	return stoi(result);
}

bool Posts::DeletePost(const std::string& postID)
{
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][DeletePost] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	m_db->DeleteFromDB("Posts", "ID_Post = '" + postID + "'");
	DeleteFromInvertedIndex(postID);
	return true;
}

bool Posts::DeleteGeneratedUserPosts(const std::string& username)
{
	if (m_UserPostsID.erase(username) && m_CounterPostsInProfile.erase(username))
	{
		m_logger->WriteLog("[Server][Posts][DeleteGenerateUserPosts] Cleared " + username + " profile queue from Server", Logger::Level::INFO);
		return true;
	}
	m_logger->WriteLog("[Server][Posts][DeleteGenerateUserPosts] " + username + " has no profile queue activated on Server", Logger::Level::WARNING);
	return false;
}

bool Posts::DeleteGeneratedCommentsFromPost(const std::string& postID)
{
	if (m_CommentsIDFromPosts.erase(postID) && m_CounterCommentsInPost.erase(postID))
	{
		m_logger->WriteLog("[Server][Posts][DeleteGeneratedCommentsFromPost] Cleared Post " + postID + " comments queue from Server", Logger::Level::INFO);
		return true;
	}
	m_logger->WriteLog("[Server][Posts][DeleteGeneratedCommentsFromPost] Post " + postID + "has no comments queue activated on Server", Logger::Level::WARNING);
	return false;
}

bool Posts::DeleteGeneratedFeed(const std::string& username)
{
	if (m_UserFeed.erase(username) && m_CounterFeed.erase(username))
	{
		m_logger->WriteLog("[Server][Posts][DeleteGeneratedFeed] Cleared " + username + " feed queue from Server", Logger::Level::INFO);
		return true;
	}
	m_logger->WriteLog("[Server][Posts][DeleteGeneratedFeed] " + username + " has no feed queue activated on Server", Logger::Level::WARNING);
	return false;
}

std::string Posts::GetUserPost(const std::string& username, const std::string& direction) //-1/0/1 Left STOP Right
{
	std::string result;
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GetUserPost] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return result;
	}
	if (m_UserPostsID.find(username) == m_UserPostsID.end())
	{
		m_UserPostsID[username] = GenerateUserPosts(username);
	}
	if (m_CounterPostsInProfile.find(username) == m_CounterPostsInProfile.end())
	{
		m_CounterPostsInProfile[username] = "1";
	}
	int order = stoi(m_CounterPostsInProfile[username]);
	short int directionIndex = stoi(direction);
	if (directionIndex == -1)
	{
		if (m_CounterPostsInProfile[username] == "1")
		{
			order = m_UserPostsID[username].size();
			m_logger->WriteLog("[Server][Posts][GetUserPost] Reached last post for " + username + " when going backwards. Resetting counter to last", Logger::Level::WARNING);

		}
		else
		{
			order--;
		}
	}
	if (directionIndex == 1)
	{
		if (order == m_UserPostsID[username].size())
		{
			order = 1;
			m_logger->WriteLog("[Server][Posts][GetUserPost] Reached last post for " + username + " when going next. Resetting counter to first", Logger::Level::WARNING);
		}
		else
		{
			order++;
		}
	}
	m_CounterPostsInProfile[username] = std::to_string(order);
	return GetPost(m_UserPostsID[username][order - 1]);
}
bool Posts::CreateComment(const std::string& postID, const std::string& username, const std::string& postedDate, const std::string& text) const
{
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][CreateComment] Post " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	std::vector<std::string> insertColumns{ "ID_Post","Username","PostedDate","Text" };
	std::vector<std::string> insertValues{ postID,username,postedDate,text };
	m_db->InsertIntoDB("Comments", insertColumns, insertValues);
	m_logger->WriteLog("[Server][Posts][CreatePost] Created comment to post = " + postID + " from User = " + username + " , and text = " + text, Logger::Level::INFO);
	return true;
}
std::string Posts::GetComment(const std::string& commentID) const
{
	std::string result = commentID;
	if (!m_db->CheckExists("Comments", "ID_Comment = '" + commentID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GetComment] Comment with ID = " + commentID + " doesn't exist in DB", Logger::Level::ERROR);
		result.clear();
		return result;
	}
	const std::vector<std::string> dbOutput = m_db->GetTemplate<std::vector<std::string>>("SELECT ID_Post,Username,PostedDate,Text FROM Comments WHERE ID_Comment = '" + commentID + "'");
	for (std::vector<std::string>::const_iterator iter = dbOutput.cbegin(); iter != dbOutput.cend(); iter++)
	{
		result = result + " " + *iter;
	}
	return result;
}

bool Posts::DeleteComment(const std::string& commentID) const
{
	if (!m_db->CheckExists("Comments", "ID_Comment = '" + commentID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][DeleteComment] Comment with ID = " + commentID + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	m_db->DeleteFromDB("Comments", "ID_Comment = '" + commentID + "'");
	return true;
}

std::string Posts::GetCommentFromPost(const std::string& postID, const std::string& direction) //-1/0/1 Left STOP Right
{
	std::string result;
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GetCommentFromPost] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return result;
	}
	if (m_CommentsIDFromPosts.find(postID) == m_CommentsIDFromPosts.end())
	{
		m_CommentsIDFromPosts[postID] = GenerateCommentsFromPost(postID);
	}
	if (m_CounterCommentsInPost.find(postID) == m_CounterCommentsInPost.end())
	{
		m_CounterCommentsInPost[postID] = "1";
	}
	int order = stoi(m_CounterCommentsInPost[postID]);
	short int directionIndex = stoi(direction);
	if (directionIndex == -1)
	{
		if (m_CounterCommentsInPost[postID] == "1")
		{
			order = m_CommentsIDFromPosts[postID].size();
			m_logger->WriteLog("[Server][Posts][GetCommentFromPost] Reached last comment for post " + postID + " when going backwards. Resetting counter to last", Logger::Level::WARNING);
		}
		else
		{
			order--;
		}
	}
	if (directionIndex == 1)
	{
		if (order == m_CommentsIDFromPosts[postID].size())
		{
			order = 1;
			m_logger->WriteLog("[Server][Posts][GetCommentFromPost] Reached last comment for post " + postID + " when going next. Resetting counter to first", Logger::Level::WARNING);
		}
		else
		{
			order++;
		}
	}
	m_CounterCommentsInPost[postID] = std::to_string(order);
	return GetComment(m_CommentsIDFromPosts[postID][order - 1]);
}

std::string Posts::GetFeed(const std::string& username, const std::string& direction)
{
	std::string result;
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GetFeed] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return result;
	}
	if (m_UserFeed.find(username) == m_UserFeed.end())
	{
		m_UserFeed[username] = GenerateFeed(username);
	}
	if (m_CounterFeed.find(username) == m_CounterFeed.end())
	{
		m_CounterFeed[username] = "1";
	}
	int order = stoi(m_CounterFeed[username]);
	short int directionIndex = stoi(direction);
	if (directionIndex == -1)
	{
		if (m_CounterFeed[username] == "1")
		{
			order = m_UserFeed[username].size();
			m_logger->WriteLog("[Server][Posts][GetFeed] Reached last post for " + username + " when going backwards. Resetting counter to last", Logger::Level::WARNING);

		}
		else
		{
			order--;
		}
	}
	if (directionIndex == 1)
	{
		if (order == m_UserFeed[username].size())
		{
			order = 1;
			m_logger->WriteLog("[Server][Posts][GetFeed] Reached last post for " + username + " when going next. Resetting counter to first", Logger::Level::WARNING);
		}
		else
		{
			order++;
		}
	}
	m_CounterFeed[username] = std::to_string(order);
	return GetPost(m_UserFeed[username][order - 1]);
}

std::string Posts::KeywordSearch(const std::string& text) const
{
	std::vector<std::string> postsSearched;
	std::vector<std::string> words;
	std::string aux(text);
	UtilityFunctions::Tokenizing(aux, words);
	using mapping = decltype(m_wordIndexes);
	for (auto word : words)
	{
		std::pair<mapping::const_iterator, mapping::const_iterator> iteratorsRange = m_wordIndexes.equal_range(word);
		for (mapping::const_iterator iter = iteratorsRange.first; iter != iteratorsRange.second; iter++)
		{
			if (std::find(postsSearched.begin(), postsSearched.end(), iter->second) == postsSearched.end())
			{
				postsSearched.push_back(iter->second);
			}
		}
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(postsSearched.begin(), postsSearched.end(), std::default_random_engine(seed));
	std::string result;
	int contor = 0;
	for (auto post : postsSearched)
	{
		result = result + post + " ";
		contor++;
	}
	if (result == "")
	{
		return "";
	}
	return result.substr(0, result.size() - 1);
}

long int Posts::GetDaysSinceCommented(const std::string& postID) const
{
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GetDaysSinceCommented] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return 0;
	}
	const std::vector<std::string> commentDates = m_db->GetTemplate<std::vector<std::string>>("SELECT PostedDate FROM Comments WHERE ID_Post = '" + postID + "'");
	if (commentDates.size() == 0)
	{
		return 1;
	}
	long int days = INT_MAX;
	for (std::vector<std::string>::const_iterator iter = commentDates.cbegin(); iter != commentDates.cend(); iter++)
	{
		long int aux = GetDaysSince(*iter);
		if (days > aux)
		{
			days = aux;
		}
	}
	return days;
}

long int Posts::GetDaysSince(const std::string& date) const
{
	tm tm = {};
	std::stringstream ss(date);
	ss >> std::get_time(&tm, "%d-%m-%Y");
	std::chrono::system_clock::time_point then =
		std::chrono::system_clock::from_time_t(mktime(&tm));
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	const int hoursInADay = 24;
	return
		std::chrono::duration_cast<std::chrono::hours>(
			now.time_since_epoch() -
			then.time_since_epoch()
			).count() / hoursInADay;
}

std::vector<std::string> Posts::GenerateUserPosts(const std::string& username)
{
	std::vector<std::string> PostsID;
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GenerateUserPosts] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return PostsID;
	}
	PostsID = m_db->GetTemplate<std::vector<std::string>>("SELECT ID_Post FROM Posts WHERE Username = '" + username + "'");
	if (PostsID.size() == 0)
	{
		m_logger->WriteLog("[Server][Posts][GenerateUserPosts] " + username + " doesn't have any posts", Logger::Level::WARNING);
		return PostsID;
	}
	m_logger->WriteLog("[Server][Posts][GenerateUserPosts] " + username + " posts generated", Logger::Level::INFO);
	return PostsID;
}

std::vector<std::string> Posts::GenerateCommentsFromPost(const std::string& postID)
{
	std::vector<std::string> CommentsID;
	if (!m_db->CheckExists("Posts", "ID_Post = '" + postID + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GenerateCommentsFromPost] Post with ID = " + postID + " doesn't exist in DB", Logger::Level::ERROR);
		return CommentsID;
	}
	CommentsID = m_db->GetTemplate<std::vector<std::string>>("SELECT ID_Comment FROM Comments WHERE ID_Post = '" + postID + "'");
	if (CommentsID.size() == 0)
	{
		m_logger->WriteLog("[Server][Posts][GenerateCommentsFromPost] Post " + postID + " doesn't have any comments", Logger::Level::WARNING);
		return CommentsID;
	}
	m_logger->WriteLog("[Server][Posts][GenerateCommentsFromPost] Post " + postID + " comments generated", Logger::Level::INFO);
	return CommentsID;
}

std::vector<std::string> Posts::GenerateFeed(const std::string& username)
{
	std::vector<std::string> postsID;
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][Posts][GenerateFeed] " + username + " doesn't exist in DB", Logger::Level::ERROR);
		return postsID;
	}
	postsID = m_db->GetTemplate<std::vector<std::string>>("SELECT ID_Post FROM Posts");
	if (postsID.size() == 0)
	{
		m_logger->WriteLog("[Server][Posts][GenerateFeed] No posts in DB", Logger::Level::WARNING);
		return postsID;
	}
	std::vector<std::vector<std::string>> dataFromDB = m_db->GetTemplate<std::vector<std::vector<std::string>>>("SELECT Username,Likes,PostedDate FROM Posts", 3);
	std::unordered_map<std::string, float> coefficientPostId;
	for (int vectorIndex = 0; vectorIndex < postsID.size(); vectorIndex++)
	{
		coefficientPostId[postsID[vectorIndex]] = (1 / (1 + GetDaysSince(dataFromDB[vectorIndex][2]) ) ) * 0.75 +
			(1 / (1 + GetDaysSinceCommented(postsID[vectorIndex]) ) ) * 0.4 + log(std::stoi(dataFromDB[vectorIndex][1]) + 1) * 0.1 +
				(m_socialGraph->CheckFriends(username, dataFromDB[vectorIndex][0]) ? 1 : 0) * 0.2;
		float variability = 0.05;
		coefficientPostId[postsID[vectorIndex]] = coefficientPostId[postsID[vectorIndex]] *
			UtilityFunctions::GenerateRandomNumber(1 - variability, 1 + variability);
	}
	auto comparator = [&coefficientPostId](const std::string& left, const std::string& right) -> bool
	{
		return coefficientPostId[left] < coefficientPostId[right];
	};
	std::priority_queue < std::string, std::vector<std::string>, decltype(comparator)> queue(comparator);
	while (!postsID.empty())
	{
		queue.emplace(std::move(postsID[0]));
		postsID.erase(postsID.begin());
	}
	while (!queue.empty())
	{
		postsID.push_back(queue.top());
		queue.pop();
	}
	m_logger->WriteLog("[Server][Posts][GenerateFeed] " + username + " posts generated", Logger::Level::INFO);
	return postsID;
}

void Posts::AddToInvertedIndex(std::vector<std::string> postsId, std::string text)
{
	if (text == "")
	{
		m_logger->WriteLog("[Server][Posts][AddToInvertedIndex] Text field was empty. Can't add posts into InvertedIndex", Logger::Level::WARNING);
		return;
	}
	using mapping = decltype(m_wordIndexes);
	text = text + " ";
	do
	{
		std::string word = text.substr(0, text.find(" "));
		std::pair<mapping::const_iterator, mapping::const_iterator> iteratorsRange = m_wordIndexes.equal_range(word);
		std::unordered_set<std::string> foundPost;
		for (mapping::const_iterator iter = iteratorsRange.first; iter != iteratorsRange.second; iter++)
		{
			for (auto id_post : postsId)
			{
				if (iter->second == id_post)
				{
					foundPost.emplace(id_post);
				}
			}
		}
		for (auto id_post : postsId)
		{
			if (foundPost.find(id_post) == foundPost.end())
			{
				m_wordIndexes.emplace(word, id_post);
			}
		}
		text = text.substr(text.find(" ") + 1);
	} while (text.find(" ") != -1);
	m_logger->WriteLog("[Server][Posts][AddToInvertedIndex] Added Posts into InvertedIndex", Logger::Level::INFO);
}

void Posts::DeleteFromInvertedIndex(std::string postId)
{
	for (std::multimap<std::string, std::string>::iterator iter = m_wordIndexes.begin(); iter != m_wordIndexes.end(); iter++)
	{
		if (iter->second == postId)
		{
			m_wordIndexes.erase(iter++);
			iter--;
		}
	}
	m_logger->WriteLog("[Server][Posts][DeleteFromInvertedIndex] Deleted Post with ID : " + postId + " from InvertedIndex", Logger::Level::INFO);
}

void Posts::InitializeInvertedIndex()
{
	std::string query = "SELECT ID_Post FROM Posts";
	std::unordered_set<std::string> posts = m_db->GetTemplate<std::unordered_set<std::string>>(query);
	for (auto post : posts)
	{
		query = "SELECT Text FROM Posts WHERE ID_Post = '" + post + "'";
		std::string text = m_db->GetTemplate<std::string>(query);
		std::vector<std::string> vectorPost{ post };
		AddToInvertedIndex(vectorPost, text);
	}
}
