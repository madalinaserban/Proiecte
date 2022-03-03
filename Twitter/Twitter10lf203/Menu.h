#pragma once
#include <iostream>
#include"ButtonSFML.h"
#include"TextBox.h"
#include "SFML/Graphics/RenderWindow.hpp"
class Menu
{

public:

	Menu();
	//Functions	
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:
	sf::Font m_font;
	sf::RectangleShape m_background;
	Textbox* m_welcome;
	//no default constructor so make it a pointer
	std::map<std::string, ButtonSFML*> m_buttons;
	//Functions
	void InitButtons(sf::RenderWindow* window);
	void InitFonts();
};

