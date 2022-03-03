#include "User.h"

bool User::Register(const std::string& email, const std::string& username, const std::string& password, const std::string& date_of_birth, const std::string& phoneNumber, const std::string& gender) const
{
	if (m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][Register] " + username + " already exists in DB", Logger::Level::WARNING);
		return false;
	}
	if (m_db->CheckExists("Users", "EMail = '" + email + "'"))
	{
		m_logger->WriteLog("[Server][User][Register] " + email + " exists in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> tableNames{ "EMail","Username","Password","BirthDate","PhoneNumber","Gender" };
	std::vector<std::string> imput{ email,username,password,date_of_birth,phoneNumber,gender };
	m_db->InsertIntoDB("Users", tableNames, imput);
	m_logger->WriteLog("[Server][User][Register] Registered User with Username = " + username + " , EMail = " + email + " , Password = " + password, Logger::Level::INFO);
	return true;
}

bool User::Login(const std::string& username, const std::string& password) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][Login] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	if (!m_db->CheckExists("Users", "Username = '" + username + "' AND Password='" + password + "'"))
	{
		m_logger->WriteLog("[Server][User][Login] " + username + " doesn't have password : " + password, Logger::Level::WARNING);
		return false;
	}
	m_logger->WriteLog("[Server][User][Login] " + username + " logged in succesfully with password : " + password, Logger::Level::INFO);
	return true;
}

bool User::ChangeUsername(const std::string& username, const std::string& newUsername) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangeUsername] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	if (m_db->CheckExists("Users", "Username = '" + newUsername + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangeUsername] " + newUsername + " already exists in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> updatedColumns{ "Username" };
	std::vector<std::string> updatedValues{ newUsername };
	std::string query = "Username = '" + username + "'";
	m_db->UpdateDB("Users", updatedColumns, updatedValues, query);
	m_logger->WriteLog("[Server][User][ChangeUsername] " + username + " changed username. New username : " + newUsername, Logger::Level::INFO);
	return true;
}

bool User::ChangeEmail(const std::string& username, const std::string& newEmail) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangeEmail] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	if (m_db->CheckExists("Users", "EMail = '" + newEmail + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangeEmail] " + newEmail + " already exists in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> updatedColumns{ "EMail" };
	std::vector<std::string> updatedValues{ newEmail };
	std::string query = "Username = '" + username + "'";
	m_db->UpdateDB("Users", updatedColumns, updatedValues, query);
	m_logger->WriteLog("[Server][User][ChangeEmail] " + username + " changed EMail. New EMail : " + newEmail, Logger::Level::INFO);
	return true;
}

bool User::ChangePassword(const std::string& username, const std::string& newPassword) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangePassword] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> updatedColumns{ "Password" };
	std::vector<std::string> updatedValues{ newPassword };
	std::string query = "Username = '" + username + "'";
	m_db->UpdateDB("Users", updatedColumns, updatedValues, query);
	m_logger->WriteLog("[Server][User][ChangePassword] " + username + " changed password. New password : " + newPassword, Logger::Level::INFO);
	return true;
}

bool User::ChangeBirthDate(const std::string& username, const std::string& newDate_of_birth) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangeBirthDate] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> updatedColumns{ "BirthDate" };
	std::vector<std::string> updatedValues{ newDate_of_birth };
	std::string query = "Username = '" + username + "'";
	m_db->UpdateDB("Users", updatedColumns, updatedValues, query);
	m_logger->WriteLog("[Server][User][ChangeBirthDate] " + username + " changed birth date. New birth date : " + newDate_of_birth, Logger::Level::INFO);
	return true;
}

bool User::ChangePhoneNumber(const std::string& username, const std::string& newPhoneNumber) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangePhoneNumber] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> updatedColumns{ "PhoneNumber" };
	std::vector<std::string> updatedValues{ newPhoneNumber };
	std::string query = "Username = '" + username + "'";
	m_db->UpdateDB("Users", updatedColumns, updatedValues, query);
	m_logger->WriteLog("[Server][User][ChangePhoneNumber] " + username + " changed phone number. New phone number : " + newPhoneNumber, Logger::Level::INFO);
	return true;
}

bool User::ChangeGender(const std::string& username, const std::string& newGender) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][ChangeGender] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	std::vector<std::string> updatedColumns{ "Gender" };
	std::vector<std::string> updatedValues{ newGender };
	std::string query = "Username = '" + username + "'";
	m_db->UpdateDB("Users", updatedColumns, updatedValues, query);
	m_logger->WriteLog("[Server][User][ChangeGender] " + username + " changed gender. New gender : " + newGender, Logger::Level::INFO);
	return true;
}

bool User::DeleteUser(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][DeleteUser] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return false;
	}
	m_db->DeleteFromDB("Users", "Username='" + username + "'");
	m_logger->WriteLog("[Server][User][DeleteUser] " + username + " account deleted", Logger::Level::INFO);
	return true;
}

std::string User::GetEmail(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][GetEmail] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return "";
	}
	m_logger->WriteLog("[Server][User][GetEmail] " + username + " Email requested", Logger::Level::INFO);
	return m_db->GetTemplate<std::string>("SELECT EMail FROM Users WHERE Username='" + username + "'");
}

std::string User::GetPassword(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][GetPassword] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return "";
	}
	m_logger->WriteLog("[Server][User][GetPassword] " + username + " password requested", Logger::Level::INFO);
	return m_db->GetTemplate<std::string>("SELECT Password FROM Users WHERE Username='" + username + "'");
}

std::string User::GetBirthDate(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger ->WriteLog("[Server][User][GetBirthDate] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return "";
	}
	m_logger->WriteLog("[Server][User][GetBirthDate] " + username + " birth date requested", Logger::Level::INFO);
	return m_db->GetTemplate<std::string>("SELECT BirthDate FROM Users WHERE Username='" + username + "'");
}

std::string User::GetPhoneNumber(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][GetPhoneNumber] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return "";
	}
	m_logger->WriteLog("[Server][User][GetPhoneNumber] " + username + " phone number requested", Logger::Level::INFO);
	return m_db->GetTemplate<std::string>("SELECT PhoneNumber FROM Users WHERE Username='" + username + "'");
}

std::string User::GetGender(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][GetGender] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return "";
	}
	m_logger->WriteLog("[Server][User][GetGender] " + username + " gender requested", Logger::Level::INFO);
	return m_db->GetTemplate<std::string>("SELECT Gender FROM Users WHERE Username='" + username + "'");
}
std::string User::GetUserData(const std::string& username) const
{
	if (!m_db->CheckExists("Users", "Username = '" + username + "'"))
	{
		m_logger->WriteLog("[Server][User][GetUserData] " + username + " doesn't exist in DB", Logger::Level::WARNING);
		return "";
	}
	std::string result;
	const std::vector<std::string> dbOutput = m_db->GetTemplate<std::vector<std::string>>("SELECT EMail,Username,Password,BirthDate,PhoneNumber,Gender FROM Users WHERE Username = '" + username + "'");
	for (std::vector<std::string>::const_iterator iter = dbOutput.cbegin(); iter != dbOutput.cend(); iter++)
	{
		result = result + *iter + " ";
	}
	return result.substr(0, result.size() - 1);
}
std::string User::UserSearch(const std::string& cuvant, short int numberOfUsernames) const
{
	std::vector<std::string> allUsernames;
	std::multimap<int, std::string> matchingUsernames;
	allUsernames = m_db->GetTemplate<std::vector<std::string>>("SELECT Username FROM Users");
	for (auto& username : allUsernames)
	{
		matchingUsernames.emplace(LevenshteinDist(cuvant, username, cuvant.length(), username.length()), std::move(username));

	}
	allUsernames.clear();
	std::string result;
	short int contor = 1;
	for (auto& it : matchingUsernames)
	{
		if (contor <= numberOfUsernames)
		{
			result = result + std::move(it.second) + " ";
			contor++;
		}
		else
		{
			break;
		}
	}
	if (result.size() == 0)
		return "";
	return result.substr(0, result.size() - 1);
}
const int User::Minim(int x, int y, int z) const
{
	return std::min(std::min(x, y), z);
}
int User::LevenshteinDist(const std::string& str1, const std::string& str2, const int size1, const int size2) const
{
	if (size1 == 0)
		return size2;
	if (size2 == 0)
		return size1;
	if (str1[size1 - 1] == str2[size2 - 1])
		return LevenshteinDist(str1, str2, size1 - 1, size2 - 1);
	return 1
		+ Minim(LevenshteinDist(str1, str2, size1, size2 - 1), // Insert
			LevenshteinDist(str1, str2, size1 - 1, size2), // Remove
			LevenshteinDist(str1, str2, size1 - 1,
				size2 - 1) // Replace
		);
}
