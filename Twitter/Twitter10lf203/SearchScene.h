#pragma once
#include"ButtonSFML.h"
#include"Post.h"

class SearchScene
{
public:
	SearchScene(bool type, std::string text, int page);
	~SearchScene();

	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:
	bool m_type;//0--word search, 1--user search
	int m_page;

	sf::Text m_title;
	std::string m_searchText;
	sf::RectangleShape m_background;
	sf::Font m_font;

	Post* m_post;
	std::map<std::string, ButtonSFML*> m_buttons;
	std::map<std::string, ButtonSFML*> m_account;
	std::map<std::string, sf::Text> m_users;

	void InitFont();
	void InitButtons(sf::RenderWindow* window);
	void InitUsers(sf::RenderWindow* window);
	void InitPost(sf::RenderWindow* window);
};

