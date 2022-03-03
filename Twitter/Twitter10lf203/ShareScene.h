#pragma once
#include"Post.h"

class ShareScene
{
public:
	ShareScene(std::string username, Post* p,sf::RenderWindow& m_beforeWindow);
	~ShareScene();

	void UpdateTypeEvent(sf::RenderWindow* window, sf::Event event);
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:

	std::string m_username;

	sf::Font m_font;
	sf::RectangleShape m_background;
	sf::RenderWindow& m_beforeWindow;
	Post* m_post;
	Textbox* m_text;
	std::map<std::string, ButtonSFML*> m_buttons;

	void InitButtons(sf::RenderWindow* window);
	void InitFonts();
};

