#pragma once
#include"Post.h"
#include "User.h"
#include "SingleTon.h"

#include <memory>
class ProfileScene
{
public:
	ProfileScene(bool type, int page);
	~ProfileScene();

	void UpdateTypeEvent(sf::RenderWindow* window, sf::Event event);
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:
	bool m_type;//0- view my posts, 1- create new post

	sf::Font m_font;
	sf::Text m_greeting;

	sf::RectangleShape m_background;

	Textbox* m_newPost;
	std::map<std::string, Post*> m_posts;
	int m_page;
	std::map<std::string, ButtonSFML*> m_buttons;


	void InitPosts(sf::RenderWindow* window);
	void InitButtons(sf::RenderWindow* window, int page);
	void InitFonts();
};

