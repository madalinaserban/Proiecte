#pragma once
#include "User.h"
#include <iostream>
#include <regex>
class LoginManager
{

public:
	enum class DataValidationMessages
	{
		eInvalidUsername,
		eInvalidEmail,
		eInvalidUsernameOrPassword,
		eInvalidBirthDate,
		eInvalidGender,
		eInvalidPhonenumber,
		eValidData
		
	};
	enum class Gender
	{
		eMale,
		eFemale,
		eOther
	};

public:

	void First_RegisterOption(const std::string user_name,const std::string email, const std::string password,std::string &text) const;
	void Second_RegisterOption(const std::string birth_date, const std::string phone_number, const std::string gender, std::string& text) const;
	const DataValidationMessages First_RegisterDataValidation(const std::string&, const std::string&, const std::string&) const;
	const DataValidationMessages Second_RegisterDataValidation(const std::string&,const std::string&,const std::string&) const;
};

