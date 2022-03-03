#pragma once
#include"ButtonSFML.h"
#include"TextBox.h"
#include"Post.h"

class CommentScene
{
public:
	CommentScene(bool type, int page,sf::RenderWindow& beforeWindow);
	~CommentScene();
	void UpdateTypeEvent(sf::RenderWindow* window, sf::Event event);
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:
	int m_page;//helps with next button
	bool m_type; //0--view comments, 1--add comm
	sf::Font m_font;
	sf::RectangleShape m_background;
	sf::RenderWindow& m_beforeWindow;
	Textbox* m_newComm;
	std::map<std::string, Post*> m_comments;
	std::map<std::string, ButtonSFML*> m_buttons;
	void InitComments(sf::RenderWindow* window);
	void InitButtons(sf::RenderWindow* window, int page);
	void InitFonts();


};

