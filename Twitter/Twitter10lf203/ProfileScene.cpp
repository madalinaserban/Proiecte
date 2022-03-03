#include "Menu.h"
#include"ShareScene.h"
#include"CommentScene.h"
#include "ProfileScene.h"
#include"MainMenuScene.h"
#include "SingleTon.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include "SingleTon.h"
#include <memory>

void ProfileScene::InitPosts(sf::RenderWindow* window)
{
	Textbox* tb = new Textbox(m_font, 24, sf::Color(232, 167, 235), false, "", sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 5), sf::Vector2f(window->getSize().x / 5.5, window->getSize().y / 5), sf::Color(128, 128, 128));
	std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
	std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
	const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETUSERPOST, importantData->GetUsername() + " 0");
	const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
	this->m_posts["id1"] = new Post(0, window, PostData[1], PostData[3], tb, &m_buttons);
	this->m_posts["id1"]->SetText(PostData[4]);
	importantData->SetPostID(PostData[0]);
}

void ProfileScene::InitButtons(sf::RenderWindow* window, int like)
{
	this->m_buttons["LOG_OUT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 2.5, window->getSize().y / 1.13), window->getSize().y / 8, window->getSize().x / 20, this->m_font, "Log out", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["EXIT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 2, window->getSize().y / 1.13), window->getSize().y / 8, window->getSize().x / 20, this->m_font, "Exit", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	if (m_type == 0)
	{
		this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(0, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
		this->m_buttons["NEXT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.1, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Next", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	}
	else
	{
		this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(0, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	}
}

void ProfileScene::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

ProfileScene::ProfileScene(bool type, int page)
{
	this->m_page = page;
	this->m_type = type;

	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1100, 900), "Twitter SFML");

	this->InitFonts();
	this->InitButtons(window, page);

	if (m_type == 0)
	{
		this->InitPosts(window);
	}
	if (m_type == 1)
	{
		this->m_buttons["SAVE_POST"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.3, window->getSize().y / 1.43), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Save", sf::Color(194, 118, 161), sf::Color(209, 79, 153), sf::Color(35, 110, 200));

		this->m_newPost = new Textbox(m_font, 24, sf::Color::White, false, "Create new post :", sf::Vector2f(window->getSize().x / 1.2, window->getSize().y / 2), sf::Vector2f(window->getSize().x / 12, window->getSize().y / 5), sf::Color(128, 128, 128));
	}

	this->m_greeting.setFont(m_font);
	this->m_greeting.setFillColor(sf::Color::White);
	this->m_greeting.setCharacterSize(24);
	this->m_greeting.setString("Welome to your profile!");
	this->m_greeting.setPosition(sf::Vector2f(window->getSize().x / 12, window->getSize().y / 12));

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

			if (m_type == 1)
				this->UpdateTypeEvent(window, *event);

		}
		this->Print(window);
	}

}

ProfileScene::~ProfileScene()
{
	if (m_type == 0)
		this->m_posts.clear();
	this->m_buttons.clear();
}

void ProfileScene::UpdateTypeEvent(sf::RenderWindow* window, sf::Event event)
{
	if (event.type == sf::Event::TextEntered)
		this->m_newPost->TypedOn(event);

	if (this->m_newPost->IsPressed(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
		this->m_newPost->SetSelected(true);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		this->m_newPost->SetSelected(false);
}

void ProfileScene::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		if (it.first != "ACCOUNT")
			it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}

	if (m_type == 0)
	{
		if (this->m_buttons["LIKE_POST"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			if (srvClient->SendMessageToServer(ServiceClient::Commands::CHECKLIKED, important->GetPostID() + " " + important->GetUsername())
				== "0")
			{
				srvClient->SendMessageToServer(ServiceClient::Commands::CHANGELIKE, important->GetPostID() + " " + important->GetUsername());
			}
		}

		if (this->m_buttons["DISLIKE_POST"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			if (srvClient->SendMessageToServer(ServiceClient::Commands::CHECKLIKED, important->GetPostID() + " " + important->GetUsername())
				== "1")
			{
				srvClient->SendMessageToServer(ServiceClient::Commands::CHANGELIKE, important->GetPostID() + " " + important->GetUsername());
			}
		}

		if (this->m_buttons["COMMENT"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::COUNTCOMMENTS, important->GetPostID());
			if (response == "0")
			{
				CommentScene c(1, 0, *window);
			}
			else
			{
				CommentScene c(0, 0, *window);
			}
		}

		if (this->m_buttons["RETWEET"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDUSERPOSTS, important->GetUsername());
			ShareScene(important->GetUsername(), m_posts["id1"], *window);
		}

		if (this->m_buttons["BACK"]->IsPressed())
		{
			window->close();
			if (m_page == 0)
			{
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
				srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDUSERPOSTS, important->GetUsername());
				MainMenuScene();
			}
			else
			{
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
				const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETUSERPOST, importantData->GetUsername() + " -1");
				const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
				importantData->SetPostID(PostData[0]);
				m_page--;
				ProfileScene(0, m_page);
			}
		}

		if (this->m_buttons["NEXT"]->IsPressed())
		{
			window->close();
			m_page++;
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETUSERPOST, importantData->GetUsername() + " 1");
			const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
			importantData->SetPostID(PostData[0]);
			ProfileScene(0, m_page);
		}

	}

	if (m_type == 1)
	{
		if (this->m_buttons["SAVE_POST"]->IsPressed())
		{
			window->close();
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::CREATEPOST, important->GetUsername() +
				" " + srvClient->GetCurrentDateToString() + " " + m_newPost->GetText() + " 0");
			MainMenuScene();
		}

		if (this->m_buttons["BACK"]->IsPressed())
		{
			window->close();
			MainMenuScene();
		}
	}

	if (this->m_buttons["LOG_OUT"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDUSERPOSTS, important->GetUsername());
		window->close();
		Menu();
	}

	if (this->m_buttons["EXIT"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDUSERPOSTS, important->GetUsername());
		window->close();
	}

}

void ProfileScene::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);
	window->draw(m_greeting);

	if (m_type == 1)
		this->m_newPost->Print(window);

	if (m_type == 0)
		for (auto& it : this->m_posts)
		{
			it.second->Print(window);
		}

	for (auto& it : this->m_buttons)
	{
		if (it.first != "ACCOUNT")
			it.second->Print(window);
	}

	window->display();
}
