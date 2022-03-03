#include "Logger.h"
#include <chrono>
#include <ctime>
Logger::Logger(Level myLevel) :
	m_maxLevel(myLevel)
{
}
void Logger::OpenClientLogFile(std::ofstream& logFile, const std::string& logFileName)
{
	if (!logFile.is_open())
	{
		std::ofstream aux_openFile(logFileName, std::ios::app);
		logFile = std::move(aux_openFile);
		aux_openFile.close();
	}
}
const std::string Logger::GetCurrentTime() const
{
	auto time = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(time);
	char currentTime_char[26];
	ctime_s(currentTime_char, sizeof(currentTime_char), &currentTime);
	std::string currentTime_string(currentTime_char);
	return currentTime_string.substr(0, currentTime_string.size() - 1);
}
void Logger::WriteLog(std::string message, Level messageLevel)
{
	if (static_cast<int>(messageLevel) >= static_cast<int>(m_maxLevel))
	{
		OpenClientLogFile(m_logFile, m_logFileName);
		m_logFile << "[" << GetCurrentTime() << "]";
		if (messageLevel == Logger::Level::INFO)
		{
			m_logFile << "[INFO] " << message << std::endl;
		}
		else if (messageLevel == Logger::Level::WARNING)
		{
			m_logFile << "[WARNING] " << message << std::endl;
		}
		else if (messageLevel == Logger::Level::ERROR)
		{
			m_logFile << "[ERROR] " << message << std::endl;
		}
		m_logFile.close();
	}
}