#include "Menu.h"
#include"RegisterLoginScene.h"

void Menu::InitButtons(sf::RenderWindow* window)
{
	ButtonSFML* b = new ButtonSFML(48, window, sf::Vector2f(window->getSize().x / 3.5, window->getSize().y / 3), window->getSize().y / 2, window->getSize().x / 7, this->m_font, "Log in", sf::Color(35, 196, 183), sf::Color(33, 161, 184), sf::Color(35, 110, 200));
	this->m_buttons.insert({ "LOGIN", b });
	//this->m_buttons["REGISTER"] = 
	ButtonSFML* u = new ButtonSFML(48, window, sf::Vector2f(window->getSize().x / 3.5, window->getSize().y / 1.9), window->getSize().y / 2, window->getSize().x / 7, this->m_font, "Register", sf::Color(128, 159, 194), sf::Color(104, 135, 171), sf::Color(35, 110, 200));
	this->m_buttons.insert({ "REGISTER",u });
	this->m_buttons["EXIT"] = new ButtonSFML(48, window, sf::Vector2f(window->getSize().x / 3.5, window->getSize().y / 1.4), window->getSize().y / 2, window->getSize().x / 7, this->m_font, "Exit", sf::Color(179, 64, 55), sf::Color(189, 31, 19), sf::Color(35, 110, 200));

}

void Menu::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

Menu::Menu()
{
	sf::RenderWindow* window;
	window = new sf::RenderWindow(sf::VideoMode(1100, 900), "Twitter SFML");

	this->InitFonts();
	this->InitButtons(window);

	this->m_welcome = new Textbox(m_font, 48, sf::Color::White, false, "Welcome!", sf::Vector2f(window->getSize().x / 3,
		window->getSize().y / 8), sf::Vector2f(window->getSize().x / 2.6, window->getSize().y / 7), sf::Color(35, 110, 200));
	
	this->m_background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	sf::Color* light_blue = new sf::Color(35, 110, 200);
	this->m_background.setFillColor(*light_blue);
	
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

void Menu::UpdateBtns(sf::RenderWindow* window)
{
	for (auto& it : this->m_buttons)
	{
		it.second->Update(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	}

	if (this->m_buttons["LOGIN"]->IsPressed())
	{
		window->close();
		RegisterLoginScene(0);
	}

	if (this->m_buttons["REGISTER"]->IsPressed())
	{
		window->close();
		RegisterLoginScene(1);
	}

	if (this->m_buttons["EXIT"]->IsPressed())
	{
		window->close();
	}
}

void Menu::Print(sf::RenderWindow* window)
{
	window->clear();

	window->draw(this->m_background);
	this->m_welcome->Print(window);

	for (auto& it : this->m_buttons)
	{
		it.second->Print(window);
	}

	window->display();
}
