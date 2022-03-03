#pragma once
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

// Define keys:
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Textbox
{
public:
	Textbox(sf::Font font,int chSize, sf::Color text_color, bool selected, std::string title, sf::Vector2f size, sf::Vector2f position, sf::Color box_color);

	void SetTitle(std::string title);

	void SetFont(sf::Font& fonts) { m_textbox.setFont(fonts); }

	void SetPosition(sf::Vector2f point) { m_textbox.setPosition(point); }

	void SetLimit(bool ToF, int lim) { m_hasLimit = ToF; m_limit = lim - 1; }

	void SetSelected(bool sel);

	std::string GetText() { return m_text.str(); }

	void DrawTo(sf::RenderWindow& window) { window.draw(m_textbox); }
	void Print(sf::RenderTarget* target);

	// Function for event loop:
	void TypedOn(sf::Event input);

	bool IsPressed(const sf::Vector2f mousePos);
	void DrawNewLine();

	sf::RectangleShape m_shape;

private:
	sf::Text m_textbox;
	sf::Text m_title;
	std::ostringstream m_text;
	sf::Font m_font;

	bool m_isSelected = false;
	bool m_hasLimit = false;
	int m_limit = 0;

	void DeleteLastChar();

	// Get user input:
	void InputLogic(int charTyped);
};