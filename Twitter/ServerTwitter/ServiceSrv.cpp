#include "ServiceSrv.h"
#include "User.h"
#include "Posts.h"
#include "SocialGraph.h"
#include "UtilityFunctions.h"
#include <chrono>

void ServiceSrv::ListenFromClient() const
{
	std::cout << "Starting server" << std::endl;

	while (true)
	{
		TcpSocket listener;
		listener.Listen(80);
		std::cout << "[" + GetCurrentTime() + "] Waiting for client to connect" << std::endl;
		TcpSocket client = listener.Accept();
		// receive
		std::array<char, 512> receiveBuffer;
		int revieved;
		client.Receive(receiveBuffer.data(), receiveBuffer.size(), revieved);
		std::cout << "[" + GetCurrentTime() + "] Received: ";
		std::copy(receiveBuffer.begin(), receiveBuffer.begin() + revieved, std::ostream_iterator<char>(std::cout, ""));
		std::cout << std::endl;
		std::string s(receiveBuffer.begin(), receiveBuffer.begin() + revieved);
		std::vector<std::string> words;
		UtilityFunctions::Tokenizing(s, words);
		// send
		CommandSelector(words, client);
	}

}
void ServiceSrv::CommandSelector(const std::vector<std::string>& arguments, TcpSocket& client) const
{
	const int command = stoi(arguments[0]);
	switch (static_cast<Commands>(command))
	{
	case Commands::LOGIN:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed LOGIN:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->Login(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::REGISTER:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed REGISTER:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->Register(arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGEUSERNAME:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGEUSERNAME:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->ChangeUsername(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGEEMAIL:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGEEMAIL:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->ChangeEmail(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGEPASSWORD:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGEPASSWORD:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->ChangePassword(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGEBIRTHDATE:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGEBIRTHDATE:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->ChangeBirthDate(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGEPHONENUMBER:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGEPHONENUMBER:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->ChangePhoneNumber(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGEGENDER:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGEGENDER:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->ChangeGender(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETEUSER:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETEUSER:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->DeleteUser(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETEMAIL:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETEMAIL:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->GetEmail(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETPASSWORD:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETPASSWORD:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->GetPassword(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETBIRTHDATE:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETBIRTHDATE:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->GetBirthDate(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETPHONENUMBER:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETPHONENUMBER:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->GetPhoneNumber(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETGENDER:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETGENDER:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->GetGender(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETUSERDATA:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETUSERDATA:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->GetUserData(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::USERSEARCH:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed USERSEARCH:\n";
		std::stringstream stream;
		std::shared_ptr<User> users = SingleTon<User>::GetInstance();
		stream << users->UserSearch(arguments[1],stoi(arguments[2]));
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHANGELIKE:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHANGELIKE:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->ChangeLike(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHECKLIKED:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHECKLIKED:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->CheckLiked(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::COUNTLIKES:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed COUNTLIKES:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->CountLikes(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CREATEPOST:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CREATEPOST:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		std::string text;
		for (int position = 3; position < arguments.size() - 1;position++)
		{
			text = text + arguments[position]+" ";
		}
		text = text.substr(0, text.size() - 1);
		stream << posts->CreatePost(arguments[1], arguments[2], text, arguments[arguments.size()-1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETPOST:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETPOST:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->GetPost(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETEPOST:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETEPOST:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->DeletePost(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETEGENERATEDUSERPOSTS:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETEGENERATEDUSERPOSTS:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->DeleteGeneratedUserPosts(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETUSERPOST:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETUSERPOST:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->GetUserPost(arguments[1],arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CREATECOMMENT:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CREATECOMMENT:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		std::string text;
		for (int position = 4; position < arguments.size(); position++)
		{
			text = text + arguments[position] + " ";
		}
		text = text.substr(0, text.size() - 1);
		stream << posts->CreateComment(arguments[1], arguments[2],arguments[3], text);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETCOMMENT:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETCOMMENT:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->GetComment(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETECOMMENT:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETECOMMENT:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->DeleteComment(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETEGENERATEDCOMMENTSFROMPOST:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETEGENERATEDCOMMENTSFROMPOST:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->DeleteGeneratedCommentsFromPost(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETCOMMENTFROMPOST:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETCOMMENTFROMPOST:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->GetCommentFromPost(arguments[1],arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::INSERTFRIENDS:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed INSERTFRIENDS:\n";
		std::stringstream stream;
		std::shared_ptr<SocialGraph> socialGraph = SingleTon<SocialGraph>::GetInstance();
		stream << socialGraph->InsertFriends(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETEFRIENDS:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETEFRIENDS:\n";
		std::stringstream stream;
		std::shared_ptr<SocialGraph> socialGraph = SingleTon<SocialGraph>::GetInstance();
		stream << socialGraph->DeleteFriends(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::CHECKFRIENDS:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed CHECKFRIENDS:\n";
		std::stringstream stream;
		std::shared_ptr<SocialGraph> socialGraph = SingleTon<SocialGraph>::GetInstance();
		stream << socialGraph->CheckFriends(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::DELETEGENERATEDFEED:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed DELETEGENERATEDFEED:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->DeleteGeneratedFeed(arguments[1]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::GETFEED:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed GETFEED:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		stream << posts->GetFeed(arguments[1], arguments[2]);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::KEYWORDSEARCH:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed KEYWORDSEARCH:\n";
		std::stringstream stream;
		std::shared_ptr<Posts> posts = SingleTon<Posts>::GetInstance();
		std::string text;
		for (int argument = 1; argument < arguments.size(); argument++)
		{
			text = text + arguments[argument] + " ";
		}
		text = text.substr(0, text.size() - 1);
		stream << posts->KeywordSearch(text);
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::COUNTPOSTS:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed COUNTPOSTS:\n";
		std::stringstream stream;
		std::shared_ptr<Database> posts = SingleTon<Database>::GetInstance();
		stream << posts->Count("Posts","Username = '"+arguments[1]+"'");
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	case Commands::COUNTCOMMENTS:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed COUNTCOMMENTS:\n";
		std::stringstream stream;
		std::shared_ptr<Database> posts = SingleTon<Database>::GetInstance();
		stream << posts->Count("Comments", "ID_Post = '" + arguments[1]+"'");
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	default:
	{
		std::cout << "[" + GetCurrentTime() + "] Accesed Wrong command : " << arguments[0];
		std::stringstream stream;
		stream << "Wrong command : " << arguments[0];
		std::string message = stream.str();
		client.Send(message.c_str(), message.size());
		std::cout << "[" + GetCurrentTime() + "] Message of " << message.size() << " bytes sent back to client: " << message.c_str() << "\n";
		break;
	}
	}
}

const std::string ServiceSrv::GetCurrentTime()
{
	auto time = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(time);
	char currentTime_char[26];
	ctime_s(currentTime_char, sizeof(currentTime_char), &currentTime);
	std::string currentTime_string(currentTime_char);
	return currentTime_string.substr(0, currentTime_string.size() - 1);
}
