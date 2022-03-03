#pragma once
#include "Database.h"
#include "SingleTon.h"
#include <memory>
class User final
{
public:
	bool Register(const std::string& email, const std::string& username, const std::string& password, const std::string& date_of_birth, const std::string& phoneNumber, const std::string& gender) const;
	bool Login(const std::string& username, const std::string& password) const;
	bool ChangeUsername(const std::string& username, const std::string& email) const;
	bool ChangeEmail(const std::string& username, const std::string& newEmail) const;
	bool ChangePassword(const std::string& username, const std::string& newPassword) const;
	bool ChangeBirthDate(const std::string& username, const std::string& newDate_of_birth) const;
	bool ChangePhoneNumber(const std::string& username, const std::string& newPhoneNumber) const;
	bool ChangeGender(const std::string& username, const std::string& newGender) const;
	bool DeleteUser(const std::string& username) const;
	std::string GetEmail(const std::string& username) const;
	std::string GetPassword(const std::string& username) const;
	std::string GetBirthDate(const std::string& username) const;
	std::string GetPhoneNumber(const std::string& username) const;
	std::string GetGender(const std::string& username) const;
	std::string GetUserData(const std::string& username) const;
	std::string UserSearch(const std::string& cuvant, short int numberOfUsernames) const;
private:

	template <class T>
	friend class SingleTon;
	User() = default;
	std::shared_ptr<Database> m_db = SingleTon<Database>::GetInstance();
	std::shared_ptr<Logger> m_logger = SingleTon<Logger>::GetInstance();
	int LevenshteinDist(const std::string& string1, const std::string& string2, const int size1, const int size2) const;
	const int Minim(int x, int y, int z) const;
};

