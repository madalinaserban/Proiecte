#include"Menu.h"
#include "FeedScene.h"
#include"ShareScene.h"
#include"CommentScene.h"
#include"MainMenuScene.h"
#include"OtherAccountScene.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include "SingleTon.h"
#include <memory>

void FeedScene::InitPosts(sf::RenderWindow* window)
{
	Textbox* tb = new Textbox(m_font, 24, sf::Color(232, 167, 235), false, "", sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 5), sf::Vector2f(window->getSize().x / 5.5, window->getSize().y / 5), sf::Color(128, 128, 128));
	std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
	std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
	const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETFEED, important->GetUsername() + " 0");
	const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
	this->m_posts["id1"] = new Post(0, window, PostData[1], PostData[3], tb, &m_buttons);
	this->m_posts["id1"]->SetText(PostData[4]);
	important->SetPostID(PostData[0]);
	important->SetOtherUsername(PostData[1]);
}

void FeedScene::InitButtons(sf::RenderWindow* window, int page)
{
	this->m_buttons["LOG_OUT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 2.5, window->getSize().y / 1.13), window->getSize().y / 8, window->getSize().x / 20, this->m_font, "Log out", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["EXIT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 2, window->getSize().y / 1.13), window->getSize().y / 8, window->getSize().x / 20, this->m_font, "Exit", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(0, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["NEXT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.1, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Next", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
}

void FeedScene::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

FeedScene::FeedScene(int feed_page)
{
	this->m_feed_page = feed_page;

	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1100, 900), "Twitter SFML");

	this->InitFonts();
	this->InitButtons(window, m_feed_page);
	this->InitPosts(window);

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

		}
		this->Print(window);
	}

}

FeedScene::~FeedScene()
{
	this->m_posts.clear();
	this->m_buttons.clear();
}

void FeedScene::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}

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

	if (this->m_buttons["RETWEET"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
		ShareScene(important->GetOtherUsername(), m_posts["id1"], *window);
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

	if (this->m_buttons["LOG_OUT"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
		window->close();
		Menu();
	}

	if (this->m_buttons["BACK"]->IsPressed())
	{
		window->close();
		if (m_feed_page == 0)
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
			MainMenuScene();
		}
		else
		{
			m_feed_page--;
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETFEED, important->GetUsername() + " -1");
			const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
			important->SetPostID(PostData[0]);
			important->SetOtherUsername(PostData[1]);
			FeedScene feed(m_feed_page);
		}
	}

	if (this->m_buttons["EXIT"]->IsPressed())
	{
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDFEED, important->GetUsername());
		window->close();
	}

	if (this->m_buttons["NEXT"]->IsPressed())
	{
		window->close();
		m_feed_page++;
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETFEED, important->GetUsername() + " 1");
		const std::vector<std::string> PostData = srvClient->TokenizingPost(response);
		important->SetPostID(PostData[0]);
		important->SetOtherUsername(PostData[1]);
		FeedScene feed(m_feed_page);
	}
}

void FeedScene::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);


	for (auto& it : this->m_posts)
	{
		it.second->Print(window);
	}

	for (auto& it : this->m_buttons)
	{
		it.second->Print(window);
	}

	window->display();
}

