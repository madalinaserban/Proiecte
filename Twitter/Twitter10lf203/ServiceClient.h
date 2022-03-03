#pragma once

#include "../Network/TcpSocket.h"
#include <array>
#include <iostream>
#include <vector>

class ServiceClient
{
public:
	enum class Commands
	{
		//Arguments after each ENUM Type, 1 space between each string
		//Dates FORMAT : DD-MM-YYYY
		LOGIN, // Arguments: Username Password || OUTPUT : 0/1 (Fail/Success)
		REGISTER, // Arguments: Username Password BirthDate PhoneNumber Gender || OUTPUT : 0/1 (Fail/Success)
		CHANGEUSERNAME, // Arguments: Username NewUsername || OUTPUT : 0/1 (Fail/Success)
		CHANGEEMAIL, // Arguments: Username NewEMail || OUTPUT : 0/1 (Fail/Success)
		CHANGEPASSWORD, // Arguments: Username NewPassword || OUTPUT : 0/1 (Fail/Success)
		CHANGEBIRTHDATE, // Arguments: Username NewBirthDate || OUTPUT : 0/1 (Fail/Success)
		CHANGEPHONENUMBER, // Arguments: Username NewPhoneNumber || OUTPUT : 0/1 (Fail/Success)
		CHANGEGENDER, // Arguments: Username NewGender || OUTPUT : 0/1 (Fail/Success)
		DELETEUSER, // Arguments: Username || OUTPUT : 0/1 (Fail/Success)
		GETEMAIL, // Arguments: Username || OUTPUT : EMail
		GETPASSWORD, // Arguments: Username || OUTPUT : EMail
		GETBIRTHDATE, // Arguments: Username || OUTPUT : EMail
		GETPHONENUMBER, // Arguments: Username || OUTPUT : EMail
		GETGENDER, // Arguments: Username || OUTPUT : EMail
		GETUSERDATA, // Arguments: Username || OUTPUT : EMail,Username,Password,BirthDate,PhoneNumber,Gender
		USERSEARCH, // Arguments: Username NumberOfUsers || OUTPUT : Usernames matching with Imput, most identic goes first.
		//Maximum NumberOfUsers Usernames in OUTPUT. Minimum 1(your username)
		//Function sorts all Usernames from DB by similarity to Username
		CHANGELIKE, // Arguments: PostID Username || OUTPUT : 0/1 (Fail/Succes at Changing Like Status)
		CHECKLIKED, // Arguments: PostID Username || OUTPUT : 0/1 (Not Liked/Liked)
		COUNTLIKES, // Arguments: PostID || OUTPUT : Number of likes
		CREATEPOST, // Arguments: Username PostedDate Text RetweetID(0 if not a Retweet, must be added) || OUTPUT : 0/1 (Fail/Succes at Creating Post)
		GETPOST, // Arguments: PostID || OUTPUT : Post Data : PostID Username Likes PostedDate Text RetweetID(0 if not a retweet)
		DELETEPOST, // Arguments: PostID || OUTPUT : 0/1 (Fail/Succes at Deleting Post)
		DELETEGENERATEDUSERPOSTS, // Arguments: Username || OUTPUT : 0/1 (Fail/Succes at Deleting Queue from User Profile of User=Username)
		//DELETEGENERATEDUSERPOSTS Practically deletes GETUSERPOST data
		GETUSERPOST, // Arguments: Username Direction(-1/0/1) || OUTPUT : Post Data from Profile: PostID Username Likes PostedDate Text RetweetID(0 if not a retweet)
		//GETUSERPOST Direction -1=Previous Post 0=(Same Post) 1=Next Post
		//GETUSERPOST starts again from last/first(direction dependent) post if reached User last post from DB.
		//GETUSERPOST generates a queue of posts wrote by User=Username if queue not existent.
		CREATECOMMENT, // Arguments: ID_Post Username PostedDate Text || OUTPUT : 0/1 (Fail/Succes at Creating Comment)
		GETCOMMENT,  // Arguments: CommentID || OUTPUT : Comment Data : CommentID ID_Post Username PostedDate Text
		DELETECOMMENT, // Arguments: CommentID || OUTPUT : 0/1 (Fail/Succes at Deleting Comment)
		DELETEGENERATEDCOMMENTSFROMPOST, //Arguments: PostID || OUTPUT : 0/1 (Fail/Succes at Deleting Queue from Post=PostID)
		//DELETEGENERATEDUSERPOSTS Practically deletes GETCOMMENTFROMPOST data
		GETCOMMENTFROMPOST,// Arguments: PostID Direction(-1/0/1) || OUTPUT : Comment Data from PostID: CommentID ID_Post Username PostedDate Text
		//GETUSERPOST Direction -1=Previous Comment 0=(Same Comment) 1=Next Comment
		//GETUSERPOST starts again from last/first(direction dependent) comment if reached PostID last Comment from DB.
		//GETUSERPOST generates a queue of comments from Post=PostID if queue not existent.
		INSERTFRIENDS, // Arguments : FirstFriend SecondFriend || OUTPUT : 0/1 (Fail/Succes at Inserting Friends)
		DELETEFRIENDS, // Arguments : FirstFriend SecondFriend || OUTPUT : 0/1 (Fail/Succes at Deleting Friends)
		CHECKFRIENDS, // Arguments : FirstFriend SecondFriend || OUTPUT : 0/1 (Not Friends/Friends)
		DELETEGENERATEDFEED, //Arguments: Username || OUTPUT : 0/1 (Fail/Succes at Deleting Feed Queue of User=Username)
		GETFEED, // Arguments: Username Direction(-1/0/1) || OUTPUT : Post Data from User=Username Feed: PostID Username Likes PostedDate Text RetweetID(0 if not a retweet)
		//GETFEED Direction -1=Previous Post 0=(Same Post) 1=Next Post
		//GETFEED starts again from last/first(direction dependent) post if reached User last post from DB.
		//GETFEED generates a queue of posts wrote by User=Username if queue not existent.
		KEYWORDSEARCH, // Arguments: Text || OUTPUT : PostsID
		COUNTPOSTS, // Arguments: Username || OUTPUT : Number of Posts User=Username posted
		COUNTCOMMENTS, // Arguments: Post_ID || OUTPUT : Number of Comments Post=Post_ID has
		NONE
	};
public:
	ServiceClient();
	std::vector<std::string> Tokenizing(std::string line) const; // Separate words by space(" ") and return in a vector
	std::vector<std::string> TokenizingPost(std::string line) const;
	// Separate string line in Format : PostID Username Likes PostedDate Text RetweetID(0 if not a retweet)
	// Used for easier future manipulation of Post Data from GETPOST(and similars)
	// SAME Output as GETPOST
	std::vector<std::string> TokenizingComment(std::string line) const;
	// Separate string line in Format : CommentID ID_Post Username PostedDate Text
	// Used for easier future manipulation of Comment Data from GETCOMMENT(and similars)
	// SAME Output as GETCOMMENT
	std::string SendMessageToServer(const Commands command, std::string arguments) const;
	static std::string GetCurrentDateToString();
private:
	const std::string GetCurrentTime() const;
};
