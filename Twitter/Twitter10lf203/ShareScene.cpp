#include "ShareScene.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include "SingleTon.h"
#include <memory>

void ShareScene::InitButtons(sf::RenderWindow* window)
{
	this->m_buttons["SHARE_POST"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 1.5), window->getSize().y / 3, window->getSize().x / 14, this->m_font, "Retweet", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
	this->m_buttons["BACK"] = new ButtonSFML(24, window, sf::Vector2f(window->getSize().x / 19, window->getSize().y / 1.5), window->getSize().y / 3, window->getSize().x / 14, this->m_font, "Back", sf::Color(72, 72, 77), sf::Color(90, 90, 99), sf::Color(35, 110, 200));
}

void ShareScene::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

ShareScene::ShareScene(std::string username, Post* p,sf::RenderWindow& beforeWindow)
	:m_post(p)
	, m_username(username),
	m_beforeWindow(beforeWindow)
{
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1000, 900), "Twitter SFML");

	m_beforeWindow.setVisible(false);
	m_beforeWindow.setActive(false);

	this->m_background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->m_background.setFillColor(sf::Color(35, 110, 200));

	this->InitFonts();
	this->InitButtons(window);

	this->m_text = new Textbox(m_font, 24, sf::Color::White, false, "Add text :", sf::Vector2f(window->getSize().x / 1.5, window->getSize().y / 5), sf::Vector2f(window->getSize().x / 5, window->getSize().y / 2.3), sf::Color(128, 128, 128));

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

ShareScene::~ShareScene()
{
	this->m_buttons.clear();
}

void ShareScene::UpdateTypeEvent(sf::RenderWindow* window, sf::Event event)
{
	if (event.type == sf::Event::TextEntered)
		this->m_text->TypedOn(event);

	if (this->m_text->IsPressed(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
		this->m_text->SetSelected(true);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		this->m_text->SetSelected(false);
}

void ShareScene::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		if (it.first != "ACCOUNT" && it.first != "LIKE" && it.first != "RETWEET" && it.first != "COMMENT" &&
			it.first != "DISLIKE")
			it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}

	if (this->m_buttons["BACK"]->IsPressed())
	{
		window->close();
		m_beforeWindow.setVisible(true);
		m_beforeWindow.setActive(true);
	}

	if (this->m_buttons["SHARE_POST"]->IsPressed())
	{
		window->close();
		std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
		std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
		srvClient->SendMessageToServer(ServiceClient::Commands::CREATEPOST, important->GetUsername() +
			" " + srvClient->GetCurrentDateToString() + " " + m_text->GetText() + " "+important->GetPostID());
		m_beforeWindow.setVisible(true);
		m_beforeWindow.setActive(true);
	}
}

void ShareScene::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);
	this->m_text->Print(window);

	m_post->Print(window);

	for (auto& it : this->m_buttons)
	{
		if (it.first != "ACCOUNT" && it.first != "LIKE" && it.first != "RETWEET" &&
			it.first != "COMMENT" && it.first != "DISLIKE")
			it.second->Print(window);
	}

	window->display();
}
