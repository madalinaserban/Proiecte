#include "MainMenuScene.h"
#include"SearchScene.h"
#include "Menu.h"
#include"FeedScene.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include "SingleTon.h"
#include <memory>

void MainMenuScene::InitButtons(sf::RenderWindow* window)
{
	this->m_buttons["EXIT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.3, window->getSize().y / 1.3), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Exit", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["LOG_OUT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 7, window->getSize().y / 1.3), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Log out", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));

	this->m_buttons["CREATE_POST"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 4, window->getSize().y / 10), window->getSize().x / 2, window->getSize().y / 9, this->m_font, "New post", sf::Color(161, 82, 222), sf::Color(139, 41, 214), sf::Color(35, 110, 200));
	this->m_buttons["PERSONAL_PROFILE"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 4, window->getSize().y / 4), window->getSize().x / 2, window->getSize().y / 9, this->m_font, "My profile", sf::Color(161, 82, 222), sf::Color(139, 41, 214), sf::Color(35, 110, 200));
	this->m_buttons["FEED"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 4, window->getSize().y / 2.5), window->getSize().x / 2, window->getSize().y / 9, this->m_font, "Feed", sf::Color(161, 82, 222), sf::Color(139, 41, 214), sf::Color(35, 110, 200));

	this->m_buttons["KEY_SEARCH"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.5 + window->getSize().x / 10, window->getSize().y / 1.8), window->getSize().x / 10, window->getSize().y / 12, this->m_font, "Search", sf::Color(161, 82, 222), sf::Color(139, 41, 214), sf::Color(35, 110, 200));
	this->m_buttons["USER_SEARCH"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.5 + window->getSize().x / 10, window->getSize().y / 1.5), window->getSize().x / 10, window->getSize().y / 12, this->m_font, "Search", sf::Color(161, 82, 222), sf::Color(139, 41, 214), sf::Color(35, 110, 200));
}

void MainMenuScene::InitTextboxes(sf::RenderWindow* window)
{
	this->m_textboxes["KEYWORD_SEARCH"] = new Textbox(m_font, 24, sf::Color::White, false, "Keyword search :", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 12), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 1.8), sf::Color(128, 128, 128));
	this->m_textboxes["USER_SEARCH"] = new Textbox(m_font, 24, sf::Color::White, false, "User search :", sf::Vector2f(window->getSize().x / 2, window->getSize().y / 12), sf::Vector2f(window->getSize().x / 4, window->getSize().y / 1.5), sf::Color(128, 128, 128));
}

void MainMenuScene::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

MainMenuScene::MainMenuScene()
{
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1100, 900), "Twitter SFML");

	this->InitFonts();
	this->InitButtons(window);
	this->InitTextboxes(window);

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

MainMenuScene::~MainMenuScene()
{
	this->m_buttons.clear();
}

void MainMenuScene::UpdateTypeEvent(sf::RenderWindow* window, sf::Event event)
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

void MainMenuScene::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}

	if (this->m_buttons["LOG_OUT"]->IsPressed())
	{
		window->close();
		Menu();
	}

	if (this->m_buttons["PERSONAL_PROFILE"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::COUNTPOSTS, important->GetUsername());
		if (response != "0")
		{
			window->close();
			ProfileScene(0, 0);
		}
	}

	if (this->m_buttons["CREATE_POST"]->IsPressed())
	{
		window->close();
		ProfileScene(1, 0);
	}

	if (this->m_buttons["FEED"]->IsPressed())
	{
		window->close();
		FeedScene(0);
	}

	if (this->m_buttons["KEY_SEARCH"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		important->SetText(m_textboxes["KEYWORD_SEARCH"]->GetText());
		const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::KEYWORDSEARCH, important->GetText());
		important->SetTextVector(srvClient->Tokenizing(response));
		if (response != "")
		{
			window->close();
			SearchScene(0, important->GetText(), 0);
		}
	}

	if (this->m_buttons["USER_SEARCH"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		important->SetText(m_textboxes["USER_SEARCH"]->GetText());
		const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::USERSEARCH, important->GetText() + " 5");
		important->SetTextVector(srvClient->Tokenizing(response)); //Vector de Usernames
		window->close();
		SearchScene(1, important->GetText(), 0);
	}

	if (this->m_buttons["EXIT"]->IsPressed())
	{
		window->close();
	}
}

void MainMenuScene::Print(sf::RenderWindow* window)
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
