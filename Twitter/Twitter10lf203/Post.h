#pragma once
#include<vector>
#include <fstream>
#include<stack>
#include<iostream>

#include"ButtonSFML.h"
#include"TextBox.h"

class Post
{
public:

	//constructors
	Post(bool type,sf::RenderWindow* window, std::string title, std::string date, Textbox* textBox, std::map<std::string, ButtonSFML*>* buttons);

	//getters
	const std::string& GetID() const;
	const std::string& GetText() const;
	const std::string& GetUserName()const;
	const sf::Vector2f& GetPositionNextBtn()const;

	//setters
	void SetText(const std::string& text);
	void SetID(const std::string& id);
	void SetUser(const std::string& user);

	//sfml functions
	//bool FinalLikePage(int page);
	void Print(sf::RenderWindow* window) const;

private:

	bool m_type;//0--post, 1--comment

	Textbox* m_textbox;
	sf::Font m_font;

	sf::Text m_likes;
	sf::Text m_likedIt;
	sf::Text m_retweeted;


	std::map<std::string, Post*> m_comments;

	sf::Text m_text; //the actual post
	std::string m_id;//the likes we get from the db, the user as well

	std::string m_userName; //the one who posted -- take from db

	//void InitLikes(sf::RenderWindow* window, int position);//db
	void InitButtons(sf::RenderWindow* window, std::map<std::string, ButtonSFML*>* buttons);
	void InitFonts();
};

