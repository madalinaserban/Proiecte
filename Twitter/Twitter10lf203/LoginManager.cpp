#include "LoginManager.h"

//TODO: schimbat numele register data validation
const LoginManager::DataValidationMessages LoginManager::First_RegisterDataValidation(const std::string& user_name, const std::string& email, const std::string& password) const
{
	if (!std::regex_match(user_name, std::regex(R"(^[a-zA-Z0-9]+([._]?[a-zA-Z0-9]+)*)"))||user_name.size()<1)
		return DataValidationMessages::eInvalidUsername;

	if (!std::regex_match(email, std::regex(R"([\w-.]+@([\w-]+.)+[\w]*)")))
		return DataValidationMessages::eInvalidEmail;

	if (!std::regex_match(password, std::regex(R"(^[a-zA-Z0-9]+([._]?[a-zA-Z0-9]+)*)"))|| password.size()<1)
		return DataValidationMessages::eInvalidUsernameOrPassword;
	

	//*SERVER CALL*  // SERVER returns true if username already in dataBase
	// - ask server if username is unique
	//if not, return DataValidation::eUserAlreadyUsed

	return DataValidationMessages::eValidData;
}

const LoginManager::DataValidationMessages LoginManager::Second_RegisterDataValidation(const std::string& birth_date, const std::string&phone_number, const std::string&gender) const
{
	if (!std::regex_match(birth_date, std::regex(R"(\d\d-\d\d-\d\d\d\d)")))
		return DataValidationMessages::eInvalidBirthDate;

	if (!std::regex_match(phone_number, std::regex(R"([0-9]*)"))|| phone_number.size()!=10)
		return DataValidationMessages::eInvalidPhonenumber;

	if (gender != "Female" && gender != "Male" && gender != "Other" )
		return DataValidationMessages::eInvalidGender;

	return DataValidationMessages::eValidData;
	
}
void LoginManager::First_RegisterOption(std::string user_name,std::string email,std::string password,std::string &text) const
{
		//Verify data
		switch (First_RegisterDataValidation(user_name, email,password))
		{

		case DataValidationMessages::eInvalidUsername:
			text= "\nInvalid username! Register again.\n";
			break;
		case DataValidationMessages::eInvalidEmail:
			text= "\nInvalid email! Register again.\n";
			break;
		case DataValidationMessages::eInvalidUsernameOrPassword:
			text= "\nInvalid Password! Register again.\n";
			break;
		case DataValidationMessages::eValidData:
			text= "Account created successfully!";
			break;
		}

	

}

void LoginManager::Second_RegisterOption(const std::string birth_date, const std::string phone_number, const std::string gender, std::string& text) const
{
	switch (Second_RegisterDataValidation(birth_date, phone_number, gender))
	{

	case DataValidationMessages::eInvalidBirthDate:
		text = "\nInvalid birth date! Register again.\n";
		break;
	case DataValidationMessages::eInvalidPhonenumber:
		text = "\nInvalid phone number! Register again.\n";
		break;
	case DataValidationMessages::eInvalidGender:
		text = "\nInvalid gender! Register again.\n";
		break;
	case DataValidationMessages::eValidData:
		text = "Account created successfully!";

		break;
	}
}
