#include "Post.h"
#include "SingleTon.h"
#include "ServiceClient.h"
#include "ImportantData.h"
#include <memory>

void Post::InitButtons(sf::RenderWindow* window, std::map<std::string, ButtonSFML*>* buttons)
{
	buttons->insert({ "ACCOUNT", new ButtonSFML(20, window, sf::Vector2f(this->m_textbox->m_shape.getPosition().x + this->m_textbox->m_shape.getSize().x, this->m_textbox->m_shape.getPosition().y),  window->getSize().x / 9, window->getSize().y / 19, this->m_font, "See profile", sf::Color(194, 118, 161),sf::Color(209, 79, 153), sf::Color(35, 110, 200)) });

	if (m_type == 0)
	{
		ButtonSFML* b = new ButtonSFML(20, window, sf::Vector2f(this->m_textbox->m_shape.getPosition().x + this->m_textbox->m_shape.getSize().x, this->m_textbox->m_shape.getPosition().y + this->m_textbox->m_shape.getSize().y), window->getSize().x / 14, window->getSize().y / 20, this->m_font, "Dislike", sf::Color(194, 118, 161), sf::Color(209, 79, 153), sf::Color(35, 110, 200));
		buttons->insert({ "DISLIKE_POST",b });
		buttons->insert({ "LIKE_POST", new ButtonSFML(20, window, sf::Vector2f(this->m_textbox->m_shape.getPosition().x + this->m_textbox->m_shape.getSize().x - window->getSize().x / 14, this->m_textbox->m_shape.getPosition().y + this->m_textbox->m_shape.getSize().y),  window->getSize().x / 14, window->getSize().y / 20, this->m_font, "Like", sf::Color(194, 118, 161),sf::Color(209, 79, 153), sf::Color(35, 110, 200)) });
		buttons->insert({ "COMMENT", new ButtonSFML(20, window, sf::Vector2f(this->m_textbox->m_shape.getPosition().x + this->m_textbox->m_shape.getSize().x - 2 * (window->getSize().x / 14) - 25, this->m_textbox->m_shape.getPosition().y + this->m_textbox->m_shape.getSize().y), window->getSize().x / 14 + 25, window->getSize().y / 20, this->m_font, "Comment",sf::Color(194, 118, 161), sf::Color(209, 79, 153), sf::Color(35, 110, 200)) });
		buttons->insert({ "RETWEET", new ButtonSFML(20, window, sf::Vector2f(this->m_textbox->m_shape.getPosition().x + this->m_textbox->m_shape.getSize().x - 3 * (window->getSize().x / 14) - 25, this->m_textbox->m_shape.getPosition().y + this->m_textbox->m_shape.getSize().y), window->getSize().x / 14, window->getSize().y / 20, this->m_font, "Retweet",sf::Color(194, 118, 161), sf::Color(209, 79, 153), sf::Color(35, 110, 200)) });
	}
}

void Post::InitFonts()
{
	if (!this->m_font.loadFromFile("Arial.ttf"))
	{
		//exits the program and tells me why
		throw("ERROR: MENUSCENE: Could not load font.");
	}
}

Post::Post(bool type, sf::RenderWindow* window, std::string username, std::string date, Textbox* textBox, std::map<std::string, ButtonSFML*>* buttons)
	:m_textbox(textBox)
	, m_type(type)
{

	std::string title = username + " (" + date + "):";
	this->m_textbox->SetTitle(title);

	this->InitFonts();
	this->InitButtons(window, buttons);

	this->m_likes.setCharacterSize(20);
	this->m_likes.setFillColor(sf::Color::White);
	this->m_likes.setFont(m_font);
	std::shared_ptr<ServiceClient> srvClient = SingleTon<ServiceClient>::GetInstance();
	std::shared_ptr<ImportantData> important = SingleTon<ImportantData>::GetInstance();
	this->m_likes.setString("Likes: " + srvClient->SendMessageToServer(ServiceClient::Commands::COUNTLIKES, important->GetPostID()));
	this->m_likes.setPosition(sf::Vector2f(m_textbox->m_shape.getPosition().x, m_textbox->m_shape.getPosition().y + m_textbox->m_shape.getSize().y));

	this->m_likedIt.setCharacterSize(20);
	this->m_likedIt.setFillColor(sf::Color::White);
	this->m_likedIt.setFont(m_font);
	this->m_likedIt.setPosition(sf::Vector2f(m_textbox->m_shape.getPosition().x + 200, m_textbox->m_shape.getPosition().y + m_textbox->m_shape.getSize().y));
	if (srvClient->SendMessageToServer(ServiceClient::Commands::CHECKLIKED, important->GetPostID() + " " + important->GetUsername()) == "1")
	{
		this->m_likedIt.setString("Liked");
	}
	else
	{
		this->m_likedIt.setString("Not Liked");
	}

	this->m_text.setFont(m_font);
	this->m_text.setFillColor(sf::Color::White);
	this->m_text.setCharacterSize(24);
	this->m_text.setPosition(sf::Vector2f(textBox->m_shape.getPosition().x + 10, textBox->m_shape.getPosition().y + 40));

}

const std::string& Post::GetID() const
{
	return m_id;
}

const std::string& Post::GetText() const
{
	return m_text.getString();
}

const std::string& Post::GetUserName() const
{
	return m_userName;
}

const sf::Vector2f& Post::GetPositionNextBtn() const
{
	return sf::Vector2f(m_textbox->m_shape.getPosition().x + m_textbox->m_shape.getSize().x
		, this->m_textbox->m_shape.getPosition().y * 2 + this->m_textbox->m_shape.getSize().y);
}

void Post::SetText(const std::string& text)
{
	this->m_text.setString(text);
}

void Post::SetID(const std::string& id)
{
	this->m_id = id;
}

void Post::SetUser(const std::string& user)
{
	this->m_userName = user;
}

void Post::Print(sf::RenderWindow* window) const
{
	this->m_textbox->Print(window);

	window->draw(this->m_text);

	window->draw(this->m_likes);

	window->draw(this->m_likedIt);

}
