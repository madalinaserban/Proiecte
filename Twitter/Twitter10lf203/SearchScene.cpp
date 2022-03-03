#include "SearchScene.h"
#include"Menu.h"
#include"MainMenuScene.h"
#include"OtherAccountScene.h"
#include"ShareScene.h"
#include"CommentScene.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include "SingleTon.h"
#include <memory>
#include <vector>


SearchScene::SearchScene(bool type, std::string text, int page)
	:m_type(type)
	, m_page(page)
	, m_searchText(text)
{
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1100, 900), "Twitter SFML");

	this->m_title.setFont(m_font);
	this->m_title.setFillColor(sf::Color::White);
	this->m_title.setCharacterSize(24);
	this->m_title.setPosition(sf::Vector2f(window->getSize().x / 12, window->getSize().y / 12));

	if (m_type == 0)
	{
		this->m_title.setString("You searched the word '" + text + "'");
	}
	else
	{
		this->m_title.setString("You searched the user '" + text + "'");
	}
	this->InitFont();
	this->InitButtons(window);

	if (m_type == 0)
	{
		this->InitPost(window);
	}
	else
	{
		this->InitUsers(window);
	}
	this->m_background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->m_background.setFillColor(sf::Color(35, 110, 200));

	while (window->isOpen())
	{
		std::shared_ptr<sf::Event> event = SingleTon<sf::Event>::GetInstance();
		while (window->pollEvent(*event))
		{
			if (event->type == sf::Event::Closed)
				window->close();
			this->UpdateBtns(window);

		}
		this->Print(window);
	}
}

SearchScene::~SearchScene()
{
	this->m_users.clear();
	this->m_buttons.clear();
}

void SearchScene::InitFont()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

void SearchScene::InitButtons(sf::RenderWindow* window)
{
	this->m_buttons["LOG_OUT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 2.5, window->getSize().y / 1.13), window->getSize().y / 8, window->getSize().x / 20, this->m_font, "Log out", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["EXIT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 2, window->getSize().y / 1.13), window->getSize().y / 8, window->getSize().x / 20, this->m_font, "Exit", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(0, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	if (m_type == 0)
	{
		this->m_buttons["NEXT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.1, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Next", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	}
}

void SearchScene::InitUsers(sf::RenderWindow* window)
{
	sf::Text user;
	user.setFont(m_font);
	user.setFillColor(sf::Color::White);
	user.setCharacterSize(24);
	user.setPosition(sf::Vector2f(window->getSize().x / 5, window->getSize().y / 8));
	std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
	std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
	const std::vector<std::string> Usernames = important->GetTextVector();
	const int number_of_users = Usernames.size();
	for (int i = 0; i < number_of_users; i++)//6 or end of users
	{
		ButtonSFML* btn = new ButtonSFML(20, window, sf::Vector2f(window->getSize().x / 12, user.getPosition().y), window->getSize().x / 9, window->getSize().y / 19, this->m_font, "See profile", sf::Color(194, 118, 161), sf::Color(209, 79, 153), sf::Color(35, 110, 200));
		this->m_account[Usernames[i]] = btn;
		m_users[Usernames[i]] = user;
		m_users[Usernames[i]].setString(Usernames[i]);
		user.setPosition(sf::Vector2f(user.getPosition().x, user.getPosition().y + 60));
	}

}

void SearchScene::InitPost(sf::RenderWindow* window)
{
	Textbox* tb = new Textbox(m_font, 24, sf::Color(232, 167, 235), false, "", sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 5), sf::Vector2f(window->getSize().x / 5.5, window->getSize().y / 5), sf::Color(128, 128, 128));
	std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
	std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
	std::vector<std::string> PostData = important->GetTextVector();
	const std::string result = srvClient->SendMessageToServer(ServiceClient::Commands::GETPOST, PostData[m_page]);
	PostData.clear();
	PostData = srvClient->TokenizingPost(result);
	m_post = new Post(0, window, PostData[1], PostData[3], tb, &m_buttons);
	m_post->SetText(PostData[4]);
	important->SetPostID(PostData[0]);
	important->SetOtherUsername(PostData[1]);
}

void SearchScene::UpdateBtns(sf::RenderWindow* window)
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

	if (m_type == 1)
	{
		if (this->m_buttons["BACK"]->IsPressed())
		{
			window->close();
			MainMenuScene();
		}
		for (auto& it : this->m_account)
		{
			it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
		}
		for (auto& it : this->m_account)
		{
			if (it.second->IsPressed())
			{
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
				important->SetOtherUsername(it.first);
				if (srvClient->SendMessageToServer(ServiceClient::Commands::COUNTPOSTS, it.first) != "0")
				{
					window->close();
					OtherAccountScene(it.first, 0);
				}
			}
		}
	}
	else
	{
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
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

		if (this->m_buttons["LIKE_POST"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			if (srvClient->SendMessageToServer(ServiceClient::Commands::CHECKLIKED, important->GetPostID() + " " + important->GetUsername())
				== "0")
			{
				srvClient->SendMessageToServer(ServiceClient::Commands::CHANGELIKE, important->GetPostID() + " " + important->GetUsername());
			}
		}

		if (this->m_buttons["ACCOUNT"]->IsPressed())
		{
			window->close();
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::COUNTPOSTS, important->GetOtherUsername());
			if (response != "0")
			{
				OtherAccountScene(important->GetOtherUsername(), 0);
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

		if (this->m_buttons["RETWEET"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
			ShareScene(important->GetOtherUsername(), m_post, *window);
		}

		if (this->m_buttons["BACK"]->IsPressed())
		{

			window->close();
			if (m_page == 0)
			{
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
				srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
				MainMenuScene();
			}
			else
			{
				m_page--;
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
				const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETPOST, important->GetTextVector()[m_page]);
				const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
				important->SetPostID(PostData[0]);
				important->SetOtherUsername(PostData[1]);
				SearchScene(m_type, m_searchText, m_page);
			}
		}

		if (this->m_buttons["NEXT"]->IsPressed() && m_page < important->GetTextVector().size() - 1)
		{
			window->close();
			m_page++;
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			const std::vector<std::string> postsID = important->GetTextVector();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETPOST, important->GetTextVector()[m_page]);
			const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
			important->SetPostID(PostData[0]);
			important->SetOtherUsername(PostData[1]);
			SearchScene(m_type, m_searchText, m_page);
		}
	}
	if (this->m_buttons["EXIT"]->IsPressed())
	{
		window->close();
	}

}

void SearchScene::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);
	window->draw(this->m_title);

	if (m_type == 1)
	{
		for (auto& it : this->m_users)
		{
			window->draw(it.second);
		}

		for (auto& it : this->m_account)
		{
			it.second->Print(window);
		}


	}
	else
	{
		m_post->Print(window);
	}

	for (auto& it : this->m_buttons)
	{
		it.second->Print(window);
	}

	window->display();
}