#pragma once
#include "sqlite3.h"
#include "UtilityFunctions.h"
#include "SingleTon.h"
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <cmath>
#include <type_traits>
#include "../Logger//Logger.h"
class Database final
{
public:
	bool Execute(const std::string query) const noexcept;
	bool CheckExists(const std::string& TableName, const std::string& Conditions = "") const noexcept;
	int Count(const std::string& TableName, const std::string& Conditions = "") const noexcept;
	void InsertIntoDB(const std::string& TableName, const std::vector<std::string>& InsertColumns, const std::vector<std::string>& Values) const noexcept;
	void UpdateDB(const std::string& TableName, const std::vector<std::string>& UpdatedColumns, const std::vector<std::string>& UpdatedValues, const std::string& Conditions = "") const noexcept;
	void DeleteFromDB(const std::string& TableName, const std::string& Conditions = "") const noexcept;
	template <class T>
	inline T GetTemplate(const std::string query, const int rows = 0);
	~Database();
private:

	template <class T>
	friend class SingleTon;
	Database();
	sqlite3* m_DB;
	std::shared_ptr<Logger> m_logger = SingleTon<Logger>::GetInstance();
	std::multimap<std::string, std::string> m_DBStructure;
	int m_openedDB;
	int OpenDatabase() noexcept;
	void InitializeDBStructure() noexcept;
	bool CheckTableName(const std::string& TableName) const noexcept;
	bool CheckColumnName(const std::string& ColumnName) const noexcept;
	// Callback Function for DB Extracting
	friend static int CallbackString(void* data, int argc, char** argv, char** azColName);
	friend static int Callback1D(void* data, int argc, char** argv, char** azColName);
	friend static int CallbackVector(void* data, int argc, char** argv, char** azColName);
};

int CallbackVector(void* data, int argc, char** argv, char** azColName)
{
	std::vector<std::string>* record = static_cast<std::vector<std::string>*>(data);
	try {
		for (int i = 0; i < argc; i++)
		{
			if (argv[i] != NULL)
			{
				record->emplace_back(std::move(argv[i]));
			}
		}
	}
	catch (...) {
	}
	return 0;
}

template<class T>
inline T Database::GetTemplate(const std::string query, const int rows)
{
	T FinalResults;
	m_logger->WriteLog("[Server][Database][GetTemplate] Wrong type. Use only allowed types(std::string types).");
	return FinalResults;
};

template<>
inline std::multimap<std::string, std::string> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	std::multimap<std::string, std::string> FinalResults;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (auxOutput.size() % 2 != 0)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Values from DB can't be mapped. Size is not even.", Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	for (std::vector<std::string>::iterator iter = auxOutput.begin(); iter != auxOutput.end(); iter++)
	{
		std::vector<std::string>::iterator aux_iter = iter;
		FinalResults.emplace(std::move(*aux_iter), std::move(*(++iter)));
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return FinalResults;
}

template<>
inline std::map<std::string, std::string> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	std::map<std::string, std::string> FinalResults;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (auxOutput.size() % 2 != 0)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Values from DB can't be mapped. Size is not even.", Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	for (std::vector<std::string>::iterator iter = auxOutput.begin(); iter != auxOutput.end(); iter++)
	{
		std::vector<std::string>::iterator aux_iter = iter;
		FinalResults.emplace(std::move(*aux_iter), std::move(*(++iter)));
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return FinalResults;
}

template<>
inline std::unordered_map<std::string, std::string> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	std::unordered_map<std::string, std::string> FinalResults;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (auxOutput.size() % 2 != 0)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Values from DB can't be mapped. Size is not even.", Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	for (std::vector<std::string>::iterator iter = auxOutput.begin(); iter != auxOutput.end(); iter++)
	{
		std::vector<std::string>::iterator aux_iter = iter;
		FinalResults.emplace(std::move(*aux_iter), std::move(*(++iter)));
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return FinalResults;
}

template<>
inline std::unordered_set<std::string> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	std::unordered_set<std::string> FinalResults;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	for (std::vector<std::string>::iterator iter = auxOutput.begin(); iter != auxOutput.end(); iter++)
	{
		FinalResults.emplace(std::move(*iter));
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return FinalResults;
}

template<>
inline std::set<std::string> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	std::set<std::string> FinalResults;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		FinalResults.clear();
		return FinalResults;
	}
	for (std::vector<std::string>::iterator iter = auxOutput.begin(); iter != auxOutput.end(); iter++)
	{
		FinalResults.emplace(std::move(*iter));
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return FinalResults;
}

template<>
inline std::vector<std::string> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
		auxOutput.clear();
		return auxOutput;
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return auxOutput;
}

template<>
inline std::string Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	char* errorMessage;
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
	}
	if (auxOutput.size() == 0)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] No value matches the query.", Logger::Level::ERROR);
		return "";
	}
	if (auxOutput.size() == 2)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Multiple values match the query. Only first will be returned.", Logger::Level::WARNING);
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return auxOutput[0];
}

template<>
inline std::vector<std::vector<std::string>> Database::GetTemplate(const std::string query, const int rows)
{
	std::vector<std::string> auxOutput;
	char* errorMessage;
	std::vector<std::vector<std::string>> FinalResults;
	if (rows <= 0)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Number of rows in a matrix can't be 0 or lower.", Logger::Level::ERROR);
		return FinalResults;
	}
	int executed = sqlite3_exec(m_DB, query.c_str(), CallbackVector, &auxOutput, &errorMessage);
	if (executed != SQLITE_OK)
	{
		m_logger->WriteLog("[Server][Database][GetTemplate] Didn't get values from DB. Error : " + (std::string)errorMessage, Logger::Level::ERROR);
	}
	std::vector<std::string> lineVectorOutput;
	for (std::vector<std::string>::iterator iter = auxOutput.begin(); iter != auxOutput.end(); iter++)
	{
		lineVectorOutput.push_back(std::move(*iter));
		if (std::distance(auxOutput.begin(), iter) % rows == rows - 1)
		{
			FinalResults.push_back(std::move(lineVectorOutput));
			lineVectorOutput.clear();
		}
	}
	m_logger->WriteLog("[Server][Database][GetTemplate] Selected succesfully. Query: " + query, Logger::Level::INFO);
	return FinalResults;
}