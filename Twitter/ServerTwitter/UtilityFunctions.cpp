#include "UtilityFunctions.h"

UtilityFunctions::UtilityFunctions() = default;

void UtilityFunctions::Tokenizing(std::string& line, std::vector<std::string>& receiver)
{
	while (line.find(" ") != -1)
	{
		receiver.push_back(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ") + 1);
	}
	receiver.push_back(line);
}
