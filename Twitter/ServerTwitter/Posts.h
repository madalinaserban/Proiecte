#pragma once
#include "Database.h"
#include "SingleTon.h"
#include "SocialGraph.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
class Posts final
{
public:
	bool ChangeLike(const std::string& postID, const std::string& username) const;
	bool CheckLiked(const std::string& postID, const std::string& username) const;
	int CountLikes(const std::string& postID) const;
	bool CreatePost(const std::string& username, const std::string& postedDate, const std::string& text, const std::string& RetweetID = "0");
	std::string GetPost(const std::string& postID) const;
	bool DeletePost(const std::string& postID);
	bool DeleteGeneratedUserPosts(const std::string& username);
	bool DeleteGeneratedCommentsFromPost(const std::string& username);
	bool DeleteGeneratedFeed(const std::string& username);
	std::string GetUserPost(const std::string& username, const std::string& direction); //-1/0/1 Left STOP Right
	bool CreateComment(const std::string& postID, const std::string& username, const std::string& postedDate, const std::string& text) const;
	std::string GetComment(const std::string& commentID) const;
	bool DeleteComment(const std::string& commentID) const;
	std::string GetCommentFromPost(const std::string& username, const std::string& direction); //-1/0/1 Left STOP Right
	std::string GetFeed(const std::string& username, const std::string& direction); //-1/0/1 Left STOP Right
	std::string KeywordSearch(const std::string& text) const;
private:

	template <class T>
	friend class SingleTon;
	Posts();
	std::shared_ptr<Database> m_db = SingleTon<Database>::GetInstance();
	std::shared_ptr<Logger> m_logger = SingleTon<Logger>::GetInstance();
	std::shared_ptr<SocialGraph> m_socialGraph = SingleTon<SocialGraph>::GetInstance();
	std::unordered_map<std::string, std::vector<std::string>> m_UserPostsID;
	std::unordered_map<std::string, std::string> m_CounterPostsInProfile;
	std::unordered_map<std::string, std::vector<std::string>> m_CommentsIDFromPosts;
	std::unordered_map<std::string, std::string> m_CounterCommentsInPost;
	std::unordered_map<std::string, std::vector<std::string>> m_UserFeed;
	std::unordered_map<std::string, std::string> m_CounterFeed;
	std::multimap<std::string, std::string> m_wordIndexes;
	std::vector<std::string> GenerateUserPosts(const std::string& username);
	std::vector<std::string> GenerateCommentsFromPost(const std::string& postID);
	std::vector<std::string> GenerateFeed(const std::string& username);
	void AddToInvertedIndex(std::vector<std::string> postId, std::string text);
	void DeleteFromInvertedIndex(std::string postId);
	void InitializeInvertedIndex();
	long int GetDaysSinceCommented(const std::string& postID) const;
	long int GetDaysSince(const std::string& dateAndHour) const;
};

