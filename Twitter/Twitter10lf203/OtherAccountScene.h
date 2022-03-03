#pragma once
#include"Post.h"

class OtherAccountScene
{
public:
	OtherAccountScene(std::string username, int page);
	~OtherAccountScene();

	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:

	std::string m_username;
	
	sf::Font m_font;
	sf::Text m_greeting;
	sf::Text m_follow;

	sf::RectangleShape m_background;

	int m_page;
	std::map<std::string, Post*> m_posts;
	std::map<std::string, ButtonSFML*> m_buttons;


	void InitPosts(sf::RenderWindow* window);
	void InitButtons(sf::RenderWindow* window, int page);
	void InitFonts();

};

