#include "ServiceClient.h"
#include <chrono>
#include <ctime>
#include <sstream>
ServiceClient::ServiceClient()
{
}
std::vector<std::string> ServiceClient::Tokenizing(std::string line) const
{
	std::vector<std::string> receiver;
	while (line.find(" ") != -1)
	{
		receiver.push_back(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ") + 1);
	}
	receiver.push_back(line);
	return receiver;
}
std::vector<std::string> ServiceClient::TokenizingPost(std::string line) const
{
	std::vector<std::string> arguments;
	std::string text;
	for (int position = 0; position <= 3; position++)
	{
		arguments.push_back(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ") + 1);
	}
	while (line.find(" ") != -1)
	{
		text = text + line.substr(0, line.find(" ")) + " ";
		line = line.substr(line.find(" ") + 1);
	}
	text = text.substr(0, text.size() - 1);
	arguments.push_back(text);
	arguments.push_back(line);
	return arguments;
}
std::vector<std::string> ServiceClient::TokenizingComment(std::string line) const
{
	std::vector<std::string> arguments;
	std::string text;
	for (int position = 0; position <= 3; position++)
	{
		arguments.push_back(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ") + 1);
	}
	while (line.find(" ") != -1)
	{
		text = text + line.substr(0, line.find(" ")) + " ";
		line = line.substr(line.find(" ") + 1);
	}
	text = text + line;
	arguments.push_back(text);
	return arguments;
}
std::string ServiceClient::SendMessageToServer(const Commands command, std::string arguments) const
{
	TcpSocket socket;
	socket.Connect("127.0.0.1", 80);
	std::string message = std::to_string(static_cast<int>(command)) + " " + arguments;
	std::cout << "[" << GetCurrentTime() << "] Sending message: " << message << std::endl;
	bool result = socket.Send(message.c_str(), message.size());
	{
		std::array<char, 512> receiveBuffer;
		int revieved;
		socket.Receive(receiveBuffer.data(), receiveBuffer.size(), revieved);
		if (revieved != -1)
		{
			std::string s(receiveBuffer.begin(), receiveBuffer.begin() + revieved);
			std::cout << "[" << GetCurrentTime() << "] Message received from Server: " << s << std::endl;
			return s;
		}
	}
	return "";
}

std::string ServiceClient::GetCurrentDateToString()
{
	auto time = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(time);
	std::tm now_tm;
	gmtime_s(&now_tm, &currentTime);
	char date[11];
	if (std::strftime(date, sizeof(date), "%d-%m-%Y", &now_tm))
	{
		std::string result(date);
		return result;
	}
	return "";
}

const std::string ServiceClient::GetCurrentTime() const
{
	auto time = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(time);
	char currentTime_char[26];
	ctime_s(currentTime_char, sizeof(currentTime_char), &currentTime);
	std::string currentTime_string(currentTime_char);
	return currentTime_string.substr(0, currentTime_string.size() - 1);
}
