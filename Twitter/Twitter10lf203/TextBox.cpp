#include "TextBox.h"

Textbox::Textbox(sf::Font font, int chSize, sf::Color text_color, bool selected, std::string title, sf::Vector2f size, sf::Vector2f position, sf::Color box_color)
	:m_isSelected(selected)
	, m_font(font)
{
	this->m_shape.setSize(size);
	this->m_shape.setFillColor(box_color);
	this->m_shape.setPosition(position);

	this->m_title.setCharacterSize(chSize);
	this->m_title.setString(title);
	this->m_title.setFont(m_font);
	this->m_title.setFillColor(text_color);
	this->m_title.setPosition(
		this->m_shape.getPosition().x + 10,
		this->m_shape.getPosition().y + 10
	);

	this->m_textbox.setFillColor(sf::Color::White);
	this->m_textbox.setCharacterSize(chSize);
	this->m_textbox.setFont(m_font);
	this->m_textbox.setPosition(
		this->m_shape.getPosition().x + 10,
		this->m_shape.getPosition().y + 40
	);

	if (m_isSelected)
		m_textbox.setString("_");
	else
		m_textbox.setString("");
}

void Textbox::SetTitle(std::string title)
{
	this->m_title.setString(title);
}

void Textbox::SetSelected(bool sel)
{
	m_isSelected = sel;

	// If not selected, remove the '_' at the end:
	if (!sel)
	{
		std::string t = m_text.str();
		std::string newT = "";
		for (int i = 0; i < t.length(); i++)
		{
			newT += t[i];
		}
		m_textbox.setString(newT);
	}
}

void Textbox::Print(sf::RenderTarget* target)
{
	target->draw(this->m_shape);
	target->draw(this->m_title);
	target->draw(this->m_textbox);
}

void Textbox::TypedOn(sf::Event input)
{
	if (m_isSelected) {

		int charTyped = input.text.unicode;

		// Only allow normal inputs:
		if (charTyped < 128)
		{
		if (m_hasLimit)
			{
				// If there's a limit, don't go over it:
				if (m_text.str().length() <= m_limit)
				{
					this->InputLogic(charTyped);
				}
				// But allow for char deletions:
				else
					//m_text.str().length() > m_limit &&
					if (m_text.str().length() < m_limit && charTyped == DELETE_KEY)
					{
						this->DeleteLastChar();
					}

				if ((m_text.str().length() % 33 == 0 && charTyped != DELETE_KEY) || charTyped==ENTER_KEY)
				{
					this->DrawNewLine();
				}
			
			}
		
			// If no limit exists, just run the function:
			else
			{
				this->InputLogic(charTyped);
				if (m_text.str().length()%33==0 && charTyped!=DELETE_KEY || charTyped == ENTER_KEY)
				{
					this->DrawNewLine();
				}
			}
		}
	}

}

bool Textbox::IsPressed(const sf::Vector2f mousePos)
{
	if (this->m_shape.getGlobalBounds().contains(mousePos))
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			return true;
	return false;
}

void Textbox::DrawNewLine()
{
	m_text << '\n';
}

void Textbox::DeleteLastChar()
{
	std::string text = m_text.str();
	std::string newText = "";
	for (int i = 0; i < text.length() - 1; i++) {
		newText += text[i];
	}
	m_text.str("");
	m_text << newText;
	m_textbox.setString(m_text.str());// +"_");
}

void Textbox::InputLogic(int charTyped)
{
	// If the key pressed isn't delete, or the two selection keys, then append the text with the char:
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY)
	{
		m_text << static_cast<char>(charTyped);
	}

	// If the key is delete, then delete the char
	else
		if (charTyped == DELETE_KEY)
		{
			if (m_text.str().length() > 0)
			{
				DeleteLastChar();
			}
		}
	// Set the textbox text:
	m_textbox.setString(m_text.str() + "_");
}
