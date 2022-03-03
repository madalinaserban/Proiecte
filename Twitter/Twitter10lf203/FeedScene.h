#pragma once
#include"Post.h"
	
class FeedScene
{
public:
	FeedScene(int feed_page);
	~FeedScene();
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:

	int m_feed_page;
	sf::Font m_font;
	sf::RectangleShape m_background;
	std::map<std::string, Post*> m_posts;
	std::map<std::string, ButtonSFML*> m_buttons;
	void InitPosts(sf::RenderWindow* window);
	void InitButtons(sf::RenderWindow* window, int page);
	void InitFonts();


};

