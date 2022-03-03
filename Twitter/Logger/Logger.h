#pragma once
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#ifdef LOGGER_EXPORTS
#define LOGGER_API _declspec(dllexport)
#else
#define LOGGER_API _declspec(dllimport)
#endif // LOGGER_EXPORT

class LOGGER_API Logger
{
public:
	enum class Level
	{
		INFO,
		WARNING,
		ERROR,
		NONE
	};
	Logger(Level myLevel = Level::INFO);
public:
	void WriteLog(std::string message, Level messageLevel);
private:
	Level m_maxLevel;
	std::ofstream m_logFile;
	const std::string m_logFileName = "../Log.txt";
	void OpenClientLogFile(std::ofstream& logFile, const std::string& logFileName);
	const std::string GetCurrentTime() const;
};

