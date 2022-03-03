#include "RegisterLoginScene.h"
#include"TextBox.h"
#include "LoginManager.h"
#include "Menu.h"
#include<fstream>
#include "ServiceClient.h"
#include "SingleTon.h"
#include "ImportantData.h"
#include <memory>
std::ofstream fout("DisplayTwitter.txt");


RegisterLoginScene::RegisterLoginScene(int type)
{
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1100, 900), "Twitter SFML");

	this->InitFonts();
	this->InitButtons(window);
	this->m_type = type;


	if (m_type == 0)
	{
		//login
		this->InitButtonsLogin(window);
		this->InitTextboxesLogin(window);
	}
	else
	{
		//register
		if (m_type == 1)
		{
			this->InitButtonsRegister1(window);
			this->InitTextboxesRegister1(window);
		}
		else
		{
			this->InitButtonsRegister2(window);
			this->InitTextboxesRegister2(window);
		}
	}

	this->m_background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->m_background.setFillColor(sf::Color(35, 110, 200));

	while (window->isOpen())
	{
		std::shared_ptr<sf::Event> event=SingleTon<sf::Event>::GetInstance();
		while (window->pollEvent(*event))
		{
			if (event->type == sf::Event::Closed)
				window->close();
			this->UpdateBtns(window);

			this->UpdateTypeEvent(window, *event);

		}
		this->Print(window);
	}

}

RegisterLoginScene::~RegisterLoginScene()
{
	this->m_buttons.clear();
	this->m_textboxes.clear();
}

void RegisterLoginScene::InitButtons(sf::RenderWindow* window)
{
	this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 7, window->getSize().y / 1.3), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
}

void RegisterLoginScene::InitButtonsLogin(sf::RenderWindow* window)
{
	this->m_buttons["LOGIN_BTN"] = new ButtonSFML(35, window, sf::Vector2f(window->getSize().x / 3.3, window->getSize().y / 1.5), window->getSize().y / 2, window->getSize().x / 17, this->m_font, "Log in", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
}

void RegisterLoginScene::InitButtonsRegister1(sf::RenderWindow* window)
{
	this->m_buttons["NEXT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.3, window->getSize().y / 1.3), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Next", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
}

void RegisterLoginScene::InitButtonsRegister2(sf::RenderWindow* window)
{
	this->m_buttons["SUBMIT"] = new ButtonSFML(35, window, sf::Vector2f(window->getSize().x / 3.3, window->getSize().y / 1.5), window->getSize().y / 2, window->getSize().x / 17, this->m_font, "Submit, then log in", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
}

void RegisterLoginScene::InitTextboxesLogin(sf::RenderWindow* window)
{
	this->m_textboxes["USERNAME"] = new Textbox(m_font, 24, sf::Color::White, false, "Username:", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 10), sf::Color(128, 128, 128));
	this->m_textboxes["PASSWORD"] = new Textbox(m_font, 24, sf::Color::White, false, "Password:", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 4), sf::Color(128, 128, 128));
}

void RegisterLoginScene::InitTextboxesRegister1(sf::RenderWindow* window)
{
	//to do set limit
	this->m_textboxes["EMAIL"] = new Textbox(m_font, 24, sf::Color::White, false, "Email:", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 10), sf::Color(128, 128, 128));
	this->m_textboxes["USERNAME"] = new Textbox(m_font, 24, sf::Color::White, false, "Username:", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 4), sf::Color(128, 128, 128));
	this->m_textboxes["PASSWORD"] = new Textbox(m_font, 24, sf::Color::White, false, "Password:", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 2.5), sf::Color(128, 128, 128));
	this->m_textboxes["CONFIRM_PASSWORD"] = new Textbox(m_font, 24, sf::Color::White, false, "Confirm password:", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 1.8), sf::Color(128, 128, 128));
}

void RegisterLoginScene::InitTextboxesRegister2(sf::RenderWindow* window)
{
	//to do set limit
	this->m_textboxes["BIRTH_DATE"] = new Textbox(m_font, 24, sf::Color::White, false, "Date of birth ( dd-mm-yyyy ):", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 10), sf::Color(128, 128, 128));
	this->m_textboxes["PHONE_NUMBER"] = new Textbox(m_font, 24, sf::Color::White, false, "Phone number (10 characters):", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 4), sf::Color(128, 128, 128));
	this->m_textboxes["GENDER"] = new Textbox(m_font, 24, sf::Color::White, false, "Gender ( Female / Male / Other ):", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 9), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 2.5), sf::Color(128, 128, 128));
}

void RegisterLoginScene::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

void RegisterLoginScene::UpdateTypeEvent(sf::RenderWindow* window, sf::Event event)
{

	for (auto& elem : m_textboxes)
	{
		if (event.type == sf::Event::TextEntered)
			elem.second->TypedOn(event);

		if (elem.second->IsPressed(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
		{
			elem.second->SetSelected(true);
			for (auto& elem2 : m_textboxes)
				if (elem != elem2)
					elem2.second->SetSelected(false);
		}

	}
}

void RegisterLoginScene::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}

	if (m_type == 0)
	{
		if (this->m_buttons["BACK"]->IsPressed())
		{
			window->close();
			Menu();
		}

		if (this->m_buttons["LOGIN_BTN"]->IsPressed())
		{
			
			std::string username = m_textboxes["USERNAME"]->GetText();
			std::string password = m_textboxes["PASSWORD"]->GetText();
			std::string arguments = username + " " + password;
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::LOGIN, arguments);
			if (response == "1")
			{
				std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
				importantData->SetUsername(username);
				user->SetUserName(username);
				window->close();
				MainMenuScene();
			}
			else
			{
				std::cout << "Login Error";
			}
		}
	}
	else
		if (m_type == 1)
		{
			if (this->m_buttons["BACK"]->IsPressed())
			{
				window->close();
				Menu();
			}

			//register buttons
			if (this->m_buttons["NEXT"]->IsPressed())
			{
				LoginManager RegisterData;
				std::string email, username, password, confirm_password, error;
				email = m_textboxes.at("EMAIL")->GetText();
				password = m_textboxes.at("PASSWORD")->GetText();
				username = m_textboxes.at("USERNAME")->GetText();
				confirm_password = m_textboxes.at("CONFIRM_PASSWORD")->GetText();
				RegisterData.First_RegisterOption(username, email, password, error);
				if (password == confirm_password && error == "Account created successfully!")
				{

					user->SetUserName(username);
					user->SetPassword(password);
					user->SetEmail(email);

					fout << user->GetEmail() << " " << user->GetUserName() << " " << user->GetPassword() << " ";
					window->close();
					RegisterLoginScene(2);
				}
				else std::cout << "\n" << error;

			}
		}
		else
		{
			if (this->m_buttons["BACK"]->IsPressed())
			{
				window->close();
				RegisterLoginScene(1);
			}

			if (this->m_buttons["SUBMIT"]->IsPressed())
			{
				LoginManager RegisterData2;
				std::string error;

				std::string birth_date, phone_number, gender;
				birth_date = this->m_textboxes["BIRTH_DATE"]->GetText();
				phone_number = m_textboxes["PHONE_NUMBER"]->GetText();
				gender = m_textboxes["GENDER"]->GetText();
				RegisterData2.Second_RegisterOption(birth_date, phone_number, gender, error);
				std::string arguments = user->GetEmail() + " " + user->GetUserName() + " " + user->GetPassword() + " " + birth_date + " " + phone_number + " " + gender;
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::REGISTER, arguments);

				if (error == "Account created successfully!" and response == "1")
				{
					fout << birth_date << " " << phone_number << " " << gender;
					window->close();
					Menu m;
				}
				else std::cout << "\n" << error;
			}
		}
}

void RegisterLoginScene::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);

	for (auto& it : this->m_buttons)
	{
		it.second->Print(window);
	}

	for (auto& it : this->m_textboxes)
	{
		it.second->Print(window);
	}

	window->display();

}
