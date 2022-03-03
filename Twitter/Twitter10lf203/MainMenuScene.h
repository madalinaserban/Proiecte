#pragma once
#include"ButtonSFML.h"
#include"ProfileScene.h"

class MainMenuScene
{
private:
	sf::Font m_font;
	sf::RectangleShape m_background;

	std::map<std::string, ButtonSFML*> m_buttons;
	std::map<std::string, Textbox*>m_textboxes;

	void InitButtons(sf::RenderWindow* window);
	void InitTextboxes(sf::RenderWindow* window);
	void InitFonts();

public:
	MainMenuScene();
	~MainMenuScene();

	void UpdateTypeEvent(sf::RenderWindow* window, sf::Event event);
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);


};

