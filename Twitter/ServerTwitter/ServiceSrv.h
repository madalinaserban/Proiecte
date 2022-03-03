#pragma once
#include "Database.h"
#include "SingleTon.h"
#include <iostream>
#include <array>
#include <sstream>
#include "../Network/TcpSocket.h"
class ServiceSrv final
{
public:
	enum class Commands
	{
		LOGIN,
		REGISTER,
		CHANGEUSERNAME,
		CHANGEEMAIL,
		CHANGEPASSWORD,
		CHANGEBIRTHDATE,
		CHANGEPHONENUMBER,
		CHANGEGENDER,
		DELETEUSER,
		GETEMAIL,
		GETPASSWORD,
		GETBIRTHDATE,
		GETPHONENUMBER,
		GETGENDER,
		GETUSERDATA,
		USERSEARCH,
		CHANGELIKE,
		CHECKLIKED,
		COUNTLIKES,
		CREATEPOST,
		GETPOST,
		DELETEPOST,
		DELETEGENERATEDUSERPOSTS,
		GETUSERPOST,
		CREATECOMMENT,
		GETCOMMENT,
		DELETECOMMENT,
		DELETEGENERATEDCOMMENTSFROMPOST,
		GETCOMMENTFROMPOST,
		INSERTFRIENDS,
		DELETEFRIENDS,
		CHECKFRIENDS,
		DELETEGENERATEDFEED,
		GETFEED,
		KEYWORDSEARCH,
		COUNTPOSTS,
		COUNTCOMMENTS,
		NONE
	};
public:
	void ListenFromClient() const;
private:
	template <class T>
	friend class SingleTon;
	ServiceSrv() = default;
	void CommandSelector(const std::vector<std::string>& arguments, TcpSocket& client) const;
	static const std::string GetCurrentTime();
};

