#pragma once
#include<string>


class User
{

public:

	User();
	User(std::string user_name,std::string password, std::string gender, std::string date);

	const std::string& GetID() const;
	const std::string& GetUserName() const;
	const std::string& GetEmail() const;
	const std::string& GetPassword() const;

	void SetID(const std::string& id);
	void SetUserName(const std::string& userName);
	void SetEmail(const std::string& email);
	void SetPassword(const std::string& pass);
	void SetGender(const std::string& gender);
	void SetBirthDate(const std::string& date);


private:
	std::string m_ID;
	std::string m_user_name;
	std::string m_email;
	std::string m_password;
	std::string m_gender;
	std::string m_birth_date;

};

