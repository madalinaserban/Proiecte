#pragma once
#include "Database.h"
#include "SingleTon.h"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
class SocialGraph final
{
public:
	bool InsertFriends(const std::string& FirstFriend, const std::string& SecondFriend);
	bool DeleteFriends(const std::string& FirstFriend, const std::string& SecondFriend);
	bool CheckFriends(const std::string& FirstFriend, const std::string& SecondFriend) const;
private:
	using MultimapStructure = std::multimap<std::string, std::string>;
	template <class T>
	friend class SingleTon;
	SocialGraph();
	MultimapStructure m_friends;
	std::shared_ptr<Database> m_db = SingleTon<Database>::GetInstance();
	std::shared_ptr<Logger> m_logger = SingleTon<Logger>::GetInstance();
	void InitializeSocialGraph();
	MultimapStructure::const_iterator FindPair(const std::string& WhoFollows, const std::string& Followed) const;
};

