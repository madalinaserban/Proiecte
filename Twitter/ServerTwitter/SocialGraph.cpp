#include "SocialGraph.h"

SocialGraph::SocialGraph()
{
	InitializeSocialGraph();
}

void SocialGraph::InitializeSocialGraph()
{
	std::string query = "SELECT FirstFriend,SecondFriend FROM Friends";
	m_friends = m_db->GetTemplate<MultimapStructure>(query);
	m_logger->WriteLog("[Server][SocialGraph][InitializeSocialGraph] Initialized SocialGraph structure", Logger::Level::INFO);
}

bool SocialGraph::InsertFriends(const std::string& FirstFriend, const std::string& SecondFriend)
{
	if (FirstFriend == SecondFriend)
	{
		m_logger->WriteLog("[Server][SocialGraph][InsertFriends] Tried to check " + FirstFriend + " with himself", Logger::Level::WARNING);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + FirstFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][InsertFriends] " + FirstFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + SecondFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][InsertFriends] " + SecondFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (FindPair(FirstFriend, SecondFriend) == m_friends.cend())
	{
		m_friends.emplace(FirstFriend, SecondFriend);
		std::string query = "INSERT INTO Friends(FirstFriend,SecondFriend) VALUES ('" + FirstFriend + "','" + SecondFriend + "')";
		m_db->Execute(query);
		m_logger->WriteLog("[Server][SocialGraph][InsertFriends] Inserted into Friends : (" + FirstFriend + "),(" + SecondFriend + ")", Logger::Level::INFO);
		return true;
	}
	m_logger->WriteLog("[Server][SocialGraph][InsertFriends] " + FirstFriend + " is already a friend of " + SecondFriend, Logger::Level::WARNING);
	return false;
}

bool SocialGraph::DeleteFriends(const std::string& FirstFriend, const std::string& SecondFriend)
{
	if (FirstFriend == SecondFriend)
	{
		m_logger->WriteLog("[Server][SocialGraph][DeleteFriends] Tried to check " + FirstFriend + " with himself", Logger::Level::WARNING);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + FirstFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][DeleteFriends] " + FirstFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + SecondFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][DeleteFriends] " + SecondFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	SocialGraph::MultimapStructure::const_iterator pair = FindPair(FirstFriend, SecondFriend);
	if (pair == m_friends.end())
	{
		m_logger->WriteLog("[Server][SocialGraph][DeleteFriends] " + FirstFriend + " is not a friend of " + SecondFriend, Logger::Level::WARNING);
		return false;
	}
	std::string query = "DELETE FROM Friends WHERE FirstFriend = '" + pair->first + "' AND SecondFriend = '" + pair->second + "'";
	m_db->Execute(query);
	m_friends.erase(pair);
	m_logger->WriteLog("[Server][SocialGraph][DeleteFriends] Deleted from Friends : (" + FirstFriend + "),(" + SecondFriend + ")", Logger::Level::INFO);
	return true;
}

bool SocialGraph::CheckFriends(const std::string& FirstFriend, const std::string& SecondFriend) const
{
	if (FirstFriend == SecondFriend)
	{
		m_logger->WriteLog("[Server][SocialGraph][CheckFriends] Tried to check " + FirstFriend + " with himself", Logger::Level::WARNING);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + FirstFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][CheckFriends] " + FirstFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + SecondFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][CheckFriends] " + SecondFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return false;
	}
	if (FindPair(FirstFriend, SecondFriend) == m_friends.cend())
	{
		m_logger->WriteLog("[Server][SocialGraph][CheckFriends] Chcked if User: " + FirstFriend + " is friend with " + SecondFriend + " . Result : 0", Logger::Level::INFO);
		return false;
	}
	m_logger->WriteLog("[Server][SocialGraph][CheckFriends] Chcked if User: " + FirstFriend + " is friend with " + SecondFriend + " . Result : 1", Logger::Level::INFO);
	return true;
}

SocialGraph::MultimapStructure::const_iterator SocialGraph::FindPair(const std::string& FirstFriend, const std::string& SecondFriend) const
{
	if (!m_db->CheckExists("Users", "Username = '" + FirstFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][FindPair] " + FirstFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return m_friends.cend();
	}
	if (!m_db->CheckExists("Users", "Username = '" + SecondFriend + "'"))
	{
		m_logger->WriteLog("[Server][SocialGraph][FindPair] " + SecondFriend + " doesn't exist in DB", Logger::Level::ERROR);
		return m_friends.cend();
	}
	std::pair<MultimapStructure::const_iterator, MultimapStructure::const_iterator> key_iterators;
	key_iterators = this->m_friends.equal_range(FirstFriend);
	for (MultimapStructure::const_iterator iter = key_iterators.first; iter != key_iterators.second; iter++)
	{
		if (iter->second == SecondFriend)
		{
			return iter;
		}
	}
	key_iterators = this->m_friends.equal_range(SecondFriend);
	for (MultimapStructure::const_iterator iter = key_iterators.first; iter != key_iterators.second; iter++)
	{
		if (iter->second == FirstFriend)
		{
			return iter;
		}
	}
	return m_friends.end();
}
