#include"Menu.h"
#include "CommentScene.h"
#include"MainMenuScene.h"
#include"OtherAccountScene.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include "SingleTon.h"
#include <memory>


void CommentScene::InitComments(sf::RenderWindow* window)
{
	Textbox* tb = new Textbox(m_font, 24, sf::Color(232, 167, 235), false, "", sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 5), sf::Vector2f(window->getSize().x / 5.5, window->getSize().y / 5), sf::Color(128, 128, 128));
	std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
	std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
	const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETCOMMENTFROMPOST, important->GetPostID() + " 0");
	const std::vector<std::string> CommentData = srvClient->TokenizingComment(response);
	this->m_comments["id2"] = new Post(1, window, CommentData[2], CommentData[3], tb, &m_buttons);
	this->m_comments["id2"]->SetText(CommentData[4]);
	important->SetCommentID(CommentData[0]);
	important->SetPostID(CommentData[1]);
	important->SetCommentUsername(CommentData[2]);
}

void CommentScene::InitButtons(sf::RenderWindow* window, int page)
{
	if (m_type == 0)
	{
		this->m_buttons["NEW_COMM"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 12), window->getSize().y / 3, window->getSize().x / 14, this->m_font, "Add comment", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));

		this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 7, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
		this->m_buttons["NEXT"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.3, window->getSize().y / 1.15), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Next", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	}
	else
	{
		this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 7, window->getSize().y / 1.43), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
		this->m_buttons["SAVE_COMM"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.3, window->getSize().y / 1.43), window->getSize().y / 8, window->getSize().x / 14, this->m_font, "Save", sf::Color(194, 118, 161), sf::Color(209, 79, 153), sf::Color(35, 110, 200));
	}
}

void CommentScene::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

CommentScene::CommentScene(bool type, int page, sf::RenderWindow& beforeWindow) :
	m_beforeWindow(beforeWindow)
{
	this->m_page = page;
	this->m_type = type;
	m_beforeWindow.setVisible(false);
	m_beforeWindow.setActive(false);
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1000, 900), "Twitter SFML");
	this->InitFonts();
	this->InitButtons(window, m_page);
	if (m_type == 0)
		this->InitComments(window);
	else
	{
		this->m_newComm = new Textbox(m_font, 24, sf::Color::White, false, "Add comment :", sf::Vector2f(window->getSize().x / 1.2, window->getSize().y / 2), sf::Vector2f(window->getSize().x / 12, window->getSize().y / 5), sf::Color(128, 128, 128));
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

			if (m_type == 1)
				this->UpdateTypeEvent(window, *event);

		}
		this->Print(window);
	}
}

CommentScene::~CommentScene()
{
	this->m_comments.clear();
	this->m_buttons.clear();
}

void CommentScene::UpdateTypeEvent(sf::RenderWindow* window, sf::Event event)
{
	if (event.type == sf::Event::TextEntered)
		this->m_newComm->TypedOn(event);

	if (this->m_newComm->IsPressed(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
		this->m_newComm->SetSelected(true);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		this->m_newComm->SetSelected(false);
}

void CommentScene::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}
	if (m_type == 0)
	{
		if (this->m_buttons["BACK"]->IsPressed())
		{
			if (m_page > 0)
			{
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
				const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETCOMMENTFROMPOST, importantData->GetPostID() + " -1");
				const std::vector<std::string> CommentData = srvClient->TokenizingComment(response);
				importantData->SetCommentID(CommentData[0]);
				importantData->SetPostID(CommentData[1]);
				importantData->SetCommentUsername(CommentData[2]);
				window->close();
				m_page--;
				CommentScene scene(0, m_page, m_beforeWindow);
			}
			else if (m_page == 0)
			{
				window->close();
				std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
				std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
				srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDCOMMENTSFROMPOST, importantData->GetPostID());
				m_beforeWindow.setVisible(true);
				m_beforeWindow.setActive(true);
			}
		}

		if (this->m_buttons["NEXT"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::GETCOMMENTFROMPOST, importantData->GetPostID() + " 1");
			const std::vector<std::string> CommentData = srvClient->TokenizingComment(response);
			importantData->SetCommentID(CommentData[0]);
			importantData->SetPostID(CommentData[1]);
			importantData->SetCommentUsername(CommentData[2]);
			window->close();
			m_page++;
			CommentScene scene(0, m_page, m_beforeWindow);
		}

		if (this->m_buttons["NEW_COMM"]->IsPressed())
		{
			window->close();
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDCOMMENTSFROMPOST, importantData->GetPostID());
			CommentScene scene(1, 0, m_beforeWindow);
		}

		if (this->m_buttons["ACCOUNT"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::DELETEGENERATEDCOMMENTSFROMPOST, important->GetUsername());
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::COUNTPOSTS, important->GetCommentUsername());
			if (response != "0")
			{
				window->close();
				m_beforeWindow.close();
				important->SetOtherUsername(important->GetCommentUsername());
				OtherAccountScene(important->GetOtherUsername(), 0);
			}
		}
	}
	else
	{
		if (this->m_buttons["BACK"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> importantData = SingleTon<ImportantData>::GetInstance();
			const std::string response = srvClient->SendMessageToServer(ServiceClient::Commands::COUNTCOMMENTS, importantData->GetPostID());
			if (response != "0")
			{
				window->close();
				CommentScene scene(0, 0, m_beforeWindow);
			}
			else
			{
				window->close();
				m_beforeWindow.setVisible(true);
				m_beforeWindow.setActive(true);
			}
		}

		if (this->m_buttons["SAVE_COMM"]->IsPressed())
		{
			std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
			std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
			srvClient->SendMessageToServer(ServiceClient::Commands::CREATECOMMENT, important->GetPostID() + " " +
				important->GetUsername() + " " + srvClient->GetCurrentDateToString() + " " + m_newComm->GetText());
			window->close();
			CommentScene scene(0, 0, m_beforeWindow);
		}
	}

}

void CommentScene::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);

	if (m_type == 0)
		for (auto& it : this->m_comments)
		{
			it.second->Print(window);
		}

	if (m_type == 1)
		this->m_newComm->Print(window);

	for (auto& it : this->m_buttons)
	{
		it.second->Print(window);
	}

	window->display();
}

