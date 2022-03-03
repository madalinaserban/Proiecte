#pragma once
#include<sstream>
#include<ctime>
//#include<cstdlib>

#include"SFML\System.hpp"
#include"SFML\Window.hpp"
#include"SFML\Graphics.hpp"	


enum button_state { btn = 0, btn_hover, btn_pressed };

class ButtonSFML
{

public:

	ButtonSFML(int chSize, sf::RenderWindow* window,sf::Vector2f r, float width, float height, sf::Font font, std::string text
		, sf::Color color, sf::Color hoverC, sf::Color pressedC);

	const bool IsPressed()const;
	sf::RectangleShape GetShape() const;
	sf::Text GetText() const;
	sf::Vector2f GetPosition() const;

	void SetPosition(sf::Vector2f pos);

	void Update(const sf::Vector2f mousePos);
	bool Update(const sf::Vector2f mousePos, const sf::RenderWindow window);
	void Print(sf::RenderWindow* w) const;


private:

	short unsigned m_buttonState;

	sf::RectangleShape m_shape;
	sf::Font m_font;
	sf::Text m_text;

	sf::Color m_color;
	sf::Color m_hoverColor;
	sf::Color m_pressColor;

};


