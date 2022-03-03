#include "User.h"

User::User() = default;

User::User(std::string user_name, std::string password, std::string gender, std::string date) :
	m_user_name(user_name),
	m_password(password),
	m_gender(gender),
	m_birth_date(date)
{
}

const std::string& User::GetID() const
{
	return m_ID;
}

const std::string& User::GetUserName() const
{
	return m_user_name;
}

const std::string& User::GetEmail() const
{
	return m_email;
}

const std::string& User::GetPassword() const
{
	return m_password;
}

void User::SetID(const std::string & id)
{
	this->m_ID = id;
}

void User::SetUserName(const std::string & userName)
{
	this->m_user_name = userName;
}

void User::SetEmail(const std::string & email)
{
	this->m_email = email;
}

void User::SetPassword(const std::string & pass)
{
	this->m_password = pass;
}

void User::SetGender(const std::string & gender)
{
	this->m_gender = gender;
}

void User::SetBirthDate(const std::string & date)
{
	this->m_birth_date = date;
}
