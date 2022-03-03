#include "ButtonSFML.h"
#include <iostream>

ButtonSFML::ButtonSFML(int chSize, sf::RenderWindow* target, sf::Vector2f pos, float width, float height, sf::Font font, std::string text
	, sf::Color color, sf::Color hoverC, sf::Color pressedC)
{
	this->m_buttonState = btn;

	this->m_shape.setPosition(pos);
	this->m_shape.setSize(sf::Vector2f(width, height));

	this->m_font = font;
	this->m_text.setFont(this->m_font);
	this->m_text.setString(text);
	this->m_text.setFillColor(sf::Color::White);
	this->m_text.setCharacterSize(chSize);


	this->m_text.setPosition(
		this->m_shape.getPosition().x + width / 2 - m_text.getCharacterSize() * ((text.size() - 1) / 3.4),
		this->m_shape.getPosition().y + height / 4 
	);


	this->m_color = color;
	this->m_hoverColor = hoverC;
	this->m_pressColor = pressedC;

	this->m_shape.setFillColor(this->m_color);
}

void ButtonSFML::Update(const sf::Vector2f mousePos)
{
	this->m_buttonState = btn;
	if (this->m_shape.getGlobalBounds().contains(mousePos))
	{
		this->m_buttonState = btn_hover;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->m_buttonState = btn_pressed;
		}
	}

	switch (this->m_buttonState)
	{
	case btn:
		this->m_shape.setFillColor(this->m_color);
		break;

	case btn_pressed:
		this->m_shape.setFillColor(this->m_pressColor);
		break;

	case btn_hover:
		this->m_shape.setFillColor(this->m_hoverColor);
		break;

	default:
		this->m_shape.setFillColor(sf::Color::Red);
		break;
	}
}

bool ButtonSFML::Update(const sf::Vector2f mousePos, const sf::RenderWindow window)
{
	bool ok = false;
	this->m_buttonState = btn;
	auto realStartPos = window.mapCoordsToPixel({ this->m_shape.getGlobalBounds().left ,this->m_shape.getGlobalBounds().top });
	auto realFinishPos = realStartPos + window.mapCoordsToPixel({ this->m_shape.getGlobalBounds().width ,this->m_shape.getGlobalBounds().height });
	if (mousePos.x > realStartPos.x && mousePos.y > realStartPos.y && mousePos.x < realFinishPos.x && mousePos.y < realFinishPos.y)
	{
		this->m_buttonState = btn_hover;
		ok = true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->m_buttonState = btn_pressed;
		}
	}
	switch (this->m_buttonState)
	{
	case btn:
		this->m_shape.setFillColor(this->m_color);
		break;

	case btn_pressed:
		this->m_shape.setFillColor(this->m_pressColor);
		break;

	case btn_hover:
		this->m_shape.setFillColor(this->m_hoverColor);
		break;

	default:
		this->m_shape.setFillColor(sf::Color::Red);
		break;
	}

	return ok;
}

void ButtonSFML::Print(sf::RenderWindow* w) const
{
	w->draw(m_shape);
	w->draw(m_text);
}

const bool ButtonSFML::IsPressed() const
{
	if (this->m_buttonState == btn_pressed)
		return true;

	return false;
}

sf::RectangleShape ButtonSFML::GetShape() const
{
	return m_shape;
}

sf::Text ButtonSFML::GetText() const
{
	return m_text;
}

sf::Vector2f ButtonSFML::GetPosition() const
{
	return m_shape.getPosition();
}

void ButtonSFML::SetPosition(sf::Vector2f pos)
{
	std::string text = m_text.getString();
	this->m_shape.setPosition(pos);
	this->m_text.setPosition(
		this->m_shape.getPosition().x + m_shape.getSize().x / 2 - m_text.getCharacterSize() * ((text.size() - 1) / 3.4),
		this->m_shape.getPosition().y + m_shape.getSize().y / 4
	);
}
