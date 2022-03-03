#include "Database.h"

Database::Database()
{
	OpenDatabase();
	InitializeDBStructure();
}

Database::~Database()
{
	sqlite3_close(this->m_DB);
	m_DBStructure.clear();
}

int CallbackString(void* data, int argc, char** argv, char** azColName)
{
	std::string* record = static_cast<std::string*>(data);
	try {
		record->push_back(**argv);
	}
	catch (...) {
	}
	return 0;
}

int Callback1D(void* data, int argc, char** argv, char** azColName)
{
	std::unordered_map<std::string, std::string>* record = static_cast<std::unordered_map<std::string, std::string>*>(data);
	try {
		for (int i = 0; i < argc; i = i + 2)
		{
			record->emplace(argv[i], argv[i + 1]);
		}
	}
	catch (...) {
	}
	return 0;
}

bool Database::CheckTableName(const std::string& TableName) const noexcept
{
	if (m_DBStructure.find(TableName) == m_DBStructure.end())
	{
		return false;
	}
	return true;
}
bool Database::CheckColumnName(const std::string& ColumnName) const noexcept
{
	for (auto iter : m_DBStructure)
	{
		if (iter.second == ColumnName)
			return true;
	}
	return false;
}

bool Database::Execute(const std::string query) const noexcept
{
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), NULL, NULL, &errorMessage);
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][Execute] Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		return false;
	}
	m_logger->WriteLog("[Server][Database][Execute]Executed query succesfully. Query : " + query, Logger::Level::INFO);
	return true;
}
bool Database::CheckExists(const std::string& TableName, const std::string& Conditions) const noexcept
{
	if (CheckTableName(TableName) == false)
	{
		m_logger->WriteLog("[Server][Database][CheckExists] Wrong TableName : " + TableName, Logger::Level::ERROR);
		return false;
	}
	std::string sqlQuery = "SELECT EXISTS(SELECT 1 FROM " + TableName;
	if (Conditions != "")
	{
		sqlQuery = sqlQuery + " WHERE " + Conditions;
	}
	sqlQuery = sqlQuery + ")";
	std::string result;
	char* errorMessage;
	sqlite3_exec(m_DB, sqlQuery.c_str(), CallbackString, &result, &errorMessage);
	if (errorMessage)
	{
		m_logger->WriteLog("[Server][Database][CheckExists] Error at checking existence into table " + TableName + " : " + (std::string)(errorMessage), Logger::Level::ERROR);
		return false;
	}
	m_logger->WriteLog("[Server][Database][CheckExists] Checked existence into table " + TableName + ". Conditions : " + Conditions, Logger::Level::INFO);
	return stoi(result) ? true : false;
}
int Database::Count(const std::string& TableName, const std::string& Conditions) const noexcept
{
	if (CheckTableName(TableName) == false)
	{
		m_logger->WriteLog("[Server][Database][Count] Wrong TableName : " + TableName, Logger::Level::ERROR);
		return -1;
	}
	std::string sqlQuery = "SELECT COUNT(*) FROM " + TableName;
	if (Conditions != "")
	{
		sqlQuery = sqlQuery + " WHERE " + Conditions;
	}
	std::string result;
	char* errorMessage;
	sqlite3_exec(m_DB, sqlQuery.c_str(), CallbackString, &result, &errorMessage);
	if (errorMessage)
	{
		m_logger->WriteLog("[Server][Database][Count] Error at counting into table " + TableName + " : " + (std::string)(errorMessage), Logger::Level::ERROR);
		return -1;
	}
	if (result.size() == 0)
	{
		return 0;
	}
	m_logger->WriteLog("[Server][Database][Count] Counted succesfully into table " + TableName + ". Conditions : " + Conditions, Logger::Level::INFO);
	return stoi(result);
}

void Database::InsertIntoDB(const std::string& TableName, const std::vector<std::string>& InsertColumns, const std::vector<std::string>& Values) const noexcept
{
	if (CheckTableName(TableName) == false)
	{
		m_logger->WriteLog("[Server][Database][InsertIntoDB] Wrong TableName : " + TableName, Logger::Level::ERROR);
		return;
	}
	if (TableName == "Friends")
	{
		m_logger->WriteLog("[Server][Database][InsertIntoDB] Use Insert Function from SocialGraph", Logger::Level::ERROR);
		return;
	}
	for (std::vector<std::string>::const_iterator iter = InsertColumns.cbegin(); iter != InsertColumns.cend(); iter++)
	{
		if (CheckColumnName(*iter) == false)
		{
			m_logger->WriteLog("[Server][Database][InsertIntoDB] One or more columns names incorrect into table " + TableName + ". Incorrect column name detected : " + *iter, Logger::Level::ERROR);
			return;
		}
	}
	if (InsertColumns.size() != Values.size() && InsertColumns.size() != 0)
	{
		m_logger->WriteLog("[Server][Database][InsertIntoDB] Wrong number of elements into parameters. Columns.size()= " + std::to_string(InsertColumns.size()) + "Values.size()= " + std::to_string(Values.size()), Logger::Level::ERROR);
		return;
	}
	std::string sqlQuery = "INSERT INTO " + TableName + "(";// +Columns + 
	for (std::vector<std::string>::const_iterator iter = InsertColumns.cbegin(); iter != InsertColumns.cend(); iter++)
	{
		sqlQuery.append(*iter);
		sqlQuery.append(",");
	}
	sqlQuery = sqlQuery.substr(0, sqlQuery.size() - 1);
	sqlQuery = sqlQuery + ") VALUES ('";
	std::string loggerSuccesMessage = "[Server][Database][InsertIntoDB] Inserted into table " + TableName + " Values :";
	for (std::vector<std::string>::const_iterator iter = Values.cbegin(); iter != Values.cend(); iter++)
	{
		sqlQuery.append(*iter);
		sqlQuery.append("','");
		loggerSuccesMessage = loggerSuccesMessage + " " + *iter;
	}
	sqlQuery = sqlQuery.substr(0, sqlQuery.size() - 2);
	sqlQuery = sqlQuery + ")";
	char* errorMessage;
	sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, NULL, &errorMessage);
	if (errorMessage)
	{
		m_logger->WriteLog("[Server][Database][InsertIntoDB] Error at inserting into DB : " + (std::string)(errorMessage), Logger::Level::ERROR);
		return;
	}
	m_logger->WriteLog(loggerSuccesMessage, Logger::Level::INFO);
}
void Database::UpdateDB(const std::string& TableName, const std::vector<std::string>& UpdatedColumns, const std::vector<std::string>& UpdatedValues, const std::string& Conditions) const noexcept
{
	if (CheckTableName(TableName) == false)
	{
		m_logger->WriteLog("[Server][Database][UpdateDB] Wrong TableName : " + TableName, Logger::Level::ERROR);
		return;
	}
	if (TableName == "Friends")
	{
		m_logger->WriteLog("[Server][Database][UpdateDB] You can't update Friends Table", Logger::Level::ERROR);
		return;
	}
	for (std::vector<std::string>::const_iterator iter = UpdatedColumns.cbegin(); iter != UpdatedColumns.cend(); iter++)
	{
		if (CheckColumnName(*iter) == false)
		{
			m_logger->WriteLog("[Server][Database][UpdateDB] One or more columns names incorrect into table " + TableName + ". Incorrect column name detected : " + *iter, Logger::Level::ERROR);
			return;
		}
	}
	if (UpdatedColumns.size() != UpdatedValues.size() && UpdatedColumns.size() != 0)
	{
		m_logger->WriteLog("[Server][Database][UpdateDB] Wrong number of elements into parameters. Columns.size()= " + std::to_string(UpdatedColumns.size()) + "Values.size()= " + std::to_string(UpdatedValues.size()), Logger::Level::ERROR);
		return;
	}
	std::string sqlQuery = "UPDATE " + TableName + " SET ";
	std::string loggerSuccesMessage = "[Server][Database][UpdateDB] Updated into table " + TableName + " columns : ";
	for (int index = 0; index < UpdatedColumns.size(); index++)
	{
		sqlQuery = sqlQuery + UpdatedColumns[index] + " = '" + UpdatedValues[index] + "', ";
		loggerSuccesMessage = loggerSuccesMessage + UpdatedColumns[index] + " = '" + UpdatedValues[index] + "', ";
	}
	sqlQuery = sqlQuery.substr(0, sqlQuery.size() - 2);
	loggerSuccesMessage = loggerSuccesMessage.substr(0, loggerSuccesMessage.size() - 2);
	if (Conditions != "")
	{
		sqlQuery = sqlQuery + " WHERE " + Conditions;
		loggerSuccesMessage = loggerSuccesMessage + " WHERE " + Conditions;
	}
	char* errorMessage;
	sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, NULL, &errorMessage);
	if (errorMessage)
	{
		m_logger->WriteLog("[Server][Database][UpdateDB] Error at updating DB : " + (std::string)(errorMessage), Logger::Level::ERROR);
		return;
	}
	m_logger->WriteLog(loggerSuccesMessage, Logger::Level::INFO);
}
void Database::DeleteFromDB(const std::string& TableName, const std::string& Conditions) const noexcept
{
	if (CheckTableName(TableName) == false)
	{
		m_logger->WriteLog("[Server][Database][DeleteFromDB] Wrong TableName : " + TableName, Logger::Level::ERROR);
		return;
	}
	if (TableName == "Friends")
	{
		m_logger->WriteLog("[Server][Database][DeleteFromDB] Use Delete Function from SocialGraph", Logger::Level::ERROR);
		return;
	}
	std::string sqlQuery = "DELETE FROM " + TableName;
	std::string loggerSuccesMessage = "[Server][Database][UpdateDB] Deleted from table " + TableName;
	if (Conditions != "")
	{
		sqlQuery = sqlQuery + " WHERE " + Conditions;
		loggerSuccesMessage = loggerSuccesMessage + " WHERE " + Conditions;
	}
	char* errorMessage;
	sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, NULL, &errorMessage);
	if (errorMessage)
	{
		m_logger->WriteLog("[Server][Database][DeleteFromDB] Error at deleting from DB : " + (std::string)(errorMessage), Logger::Level::ERROR);
		return;
	}
	m_logger->WriteLog(loggerSuccesMessage, Logger::Level::INFO);
}

void Database::InitializeDBStructure() noexcept
{
	std::multimap<std::string, std::string> DBStructure{
		{"Users", "Username"}, {"Users", "EMail"}, {"Users", "Password"},
		{"Users", "BirthDate"}, {"Users", "PhoneNumber"}, {"Users", "Gender"},
		{"Friends", "FirstFriend"}, {"Friends", "SecondFriend"},
		{"Posts", "ID_Post"}, {"Posts", "ID_Retweet"}, {"Posts", "Username"}, {"Posts", "Text"},
		{"Posts", "Likes"}, {"Posts", "PostedDate"},
		{"Comments", "ID_Comment"}, {"Comments", "ID_Post"},{"Comments", "Username"}, {"Comments", "Text"},
		{"Comments", "PostedDate"}, {"LikesDislikes", "ID_Post"},{"LikesDislikes", "Username"} };
	m_DBStructure = std::move(DBStructure);
	m_logger->WriteLog("[Server][Database][InitializeDBStructure] Initialized DB structure", Logger::Level::INFO);
}
int Database::OpenDatabase() noexcept
{
	const std::string fileName = "../Database/TwitterDatabase.db";
	if (m_openedDB == 0)
	{
		if (!std::filesystem::exists(fileName))
		{
			m_logger->WriteLog("[Server][Database][OpenDatabase] " + fileName + " doesn't exist. Creating " + fileName, Logger::Level::WARNING);
			m_openedDB = sqlite3_open(fileName.c_str(), &m_DB);
			if (m_openedDB != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at opening " + fileName, Logger::Level::ERROR);
				return (-1);
			}
			std::string sqlQuery = "CREATE TABLE Users( "
				"Username    STRING(50) NOT NULL ON CONFLICT FAIL "
				"PRIMARY KEY ON CONFLICT FAIL "
				"UNIQUE ON CONFLICT FAIL "
				"CHECK(LENGTH(Username) <= 50), "
				"EMail       STRING(60) UNIQUE ON CONFLICT FAIL "
				"NOT NULL ON CONFLICT FAIL "
				"CHECK(EMail GLOB '?*@?*.?*' AND "
				"LENGTH(EMail) <= 60), "
				"Password    STRING(40) NOT NULL ON CONFLICT FAIL "
				"CHECK(LENGTH(Password) <= 40), "
				"BirthDate   DATE        NOT NULL ON CONFLICT FAIL "
				"CHECK(BirthDate GLOB '[0-3][0-9]-[0-1][0-9]-[0-9][0-9][0-9][0-9]'), "
				"PhoneNumber STRING(15) NOT NULL ON CONFLICT FAIL "
				"CHECK(NOT PhoneNumber GLOB '*[^+0-9]*' AND "
				"PhoneNumber LIKE '_%'), "
				"Gender      STRING(6)  NOT NULL ON CONFLICT FAIL "
				"CHECK(Gender IN('Male', 'Female', 'Other')) "
				"); ";
			int succesfullQuerry = 0;
			char* messageError;
			succesfullQuerry = sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, 0, &messageError);
			if (succesfullQuerry != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at Users table creation", Logger::Level::ERROR);
				sqlite3_free(messageError);
				return (-1);
			}
			else
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Users table created succesfully", Logger::Level::INFO);
			}
			sqlQuery = "CREATE TABLE Friends( "
				"FirstFriend  STRING(50) REFERENCES Users(Username) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"NOT NULL ON CONFLICT FAIL, "
				"SecondFriend STRING(50) REFERENCES Users(Username) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"NOT NULL ON CONFLICT FAIL, "
				"UNIQUE( "
				"FirstFriend, "
				"SecondFriend "
				") "
				"ON CONFLICT FAIL, "
				"CHECK(FirstFriend != SecondFriend) "
				"); ";
			succesfullQuerry = sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, 0, &messageError);
			if (succesfullQuerry != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at Follow table creation", Logger::Level::ERROR);
				sqlite3_free(messageError);
				return (-1);
			}
			else
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Follow table created succesfully", Logger::Level::INFO);
			}
			sqlQuery = "CREATE TABLE Posts( "
				"ID_Post    INTEGER      PRIMARY KEY ON CONFLICT FAIL AUTOINCREMENT "
				"UNIQUE ON CONFLICT FAIL "
				"NOT NULL ON CONFLICT FAIL "
				"CHECK(ID_Post >= 0), "
				"ID_Retweet INTEGER      REFERENCES Posts(ID_Post) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"CHECK(ID_Retweet > 0), "
				"Username   STRING(50)  REFERENCES Users(Username) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"NOT NULL ON CONFLICT FAIL, "
				"Text       STRING(300) NOT NULL ON CONFLICT FAIL "
				"CHECK(LENGTH(Text) <= 300), "
				"Likes      INT          NOT NULL ON CONFLICT FAIL "
				"CHECK(Likes >= 0) "
				"DEFAULT(0), "
				"PostedDate DATE         CHECK(PostedDate GLOB '[0-3][0-9]-[0-1][0-9]-[0-9][0-9][0-9][0-9]') "
				"NOT NULL ON CONFLICT FAIL, "
				"CHECK(ID_Retweet != ID_Post) "
				"); ";
			succesfullQuerry = sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, 0, &messageError);
			if (succesfullQuerry != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at Posts table creation", Logger::Level::ERROR);
				sqlite3_free(messageError);
				return (-1);
			}
			else
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Posts table created succesfully", Logger::Level::INFO);
			}
			sqlQuery = "CREATE TABLE Comments( "
				"ID_Comment INTEGER      PRIMARY KEY ON CONFLICT FAIL AUTOINCREMENT "
				"NOT NULL ON CONFLICT FAIL "
				"CHECK(ID_Comment > 0) "
				"UNIQUE ON CONFLICT FAIL, "
				"ID_Post    INTEGER      REFERENCES Posts(ID_Post) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"CHECK(ID_Post > 0), "
				"Username   STRING(50)  NOT NULL ON CONFLICT FAIL "
				"REFERENCES Users(Username) ON DELETE CASCADE "
				"ON UPDATE CASCADE, "
				"Text       STRING(300) NOT NULL ON CONFLICT FAIL "
				"CHECK(LENGTH(Text) <= 300), "
				"PostedDate DATE         CHECK(PostedDate GLOB '[0-3][0-9]-[0-1][0-9]-[0-9][0-9][0-9][0-9]') "
				"NOT NULL ON CONFLICT FAIL "
				"); ";
			succesfullQuerry = sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, 0, &messageError);
			if (succesfullQuerry != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at comments table creation", Logger::Level::ERROR);
				sqlite3_free(messageError);
				return (-1);
			}
			else
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Comments table created succesfully", Logger::Level::INFO);
			}
			sqlQuery = "CREATE TABLE LikesDislikes( "
				"ID_Post  INTEGER     REFERENCES Posts(ID_Post) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"NOT NULL, "
				"Username STRING(50) REFERENCES Users(Username) ON DELETE CASCADE "
				"ON UPDATE CASCADE "
				"NOT NULL ON CONFLICT FAIL, "
				"PRIMARY KEY( "
				"ID_Post, "
				"Username "
				") "
				"ON CONFLICT FAIL, "
				"UNIQUE( "
				"ID_Post, "
				"Username "
				") "
				"ON CONFLICT FAIL "
				"); ";
			succesfullQuerry = sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, 0, &messageError);
			if (succesfullQuerry != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at LikesDislikes table creation", Logger::Level::ERROR);
				sqlite3_free(messageError);
				return (-1);
			}
			else
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] LikesDislikes table created succesfully", Logger::Level::INFO);
			}
			sqlQuery = "CREATE TRIGGER InsertLikeDislike "
				"AFTER INSERT "
				"ON LikesDislikes "
				"BEGIN "
				"UPDATE Posts "
				"SET Likes = ( "
				"SELECT COUNT(*) "
				"FROM LikesDislikes "
				"WHERE ID_Post = Posts.ID_Post "
				"); "
				"END; "
				"CREATE TRIGGER UpdateLikeDislike "
				"AFTER UPDATE "
				"ON LikesDislikes "
				"BEGIN "
				"UPDATE Posts "
				"SET Likes = ( "
				"SELECT COUNT(*) "
				"FROM LikesDislikes "
				"WHERE ID_Post = Posts.ID_Post "
				"); "
				"END; "
				"CREATE TRIGGER DeleteLikeDislike "
				"AFTER DELETE "
				"ON LikesDislikes "
				"BEGIN "
				"UPDATE Posts "
				"SET Likes = ( "
				"SELECT COUNT(*) "
				"FROM LikesDislikes "
				"WHERE ID_Post = Posts.ID_Post "
				"); "
				"END; ";
			succesfullQuerry = sqlite3_exec(m_DB, sqlQuery.c_str(), NULL, 0, &messageError);
			if (succesfullQuerry != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at triggers from LikesDislikes table creation", Logger::Level::ERROR);
				sqlite3_free(messageError);
				return (-1);
			}
			else
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Triggers from LikesDislike table created succesfully", Logger::Level::INFO);
			}
			return 0;
		}
		else
		{
			m_openedDB = sqlite3_open(fileName.c_str(), &m_DB);
			if (m_openedDB != SQLITE_OK)
			{
				m_logger->WriteLog("[Server][Database][OpenDatabase] Error at opening " + fileName, Logger::Level::ERROR);
				return (-1);
			}
			m_logger->WriteLog("[Server][Database][OpenDatabase] Opened " + fileName + " database succesfully.", Logger::Level::INFO);
			return 0;
		}
	}
	return 0;
}
