#pragma once
#include"MainMenuScene.h"
#include"TextBox.h"
#include"ButtonSFML.h"
#include"User.h"
#include "SingleTon.h"
#include <memory>
//enum scene_type { login, register1, register2 };
//TODO :intai public , dupa privat !
//TODO: METODE CU LITERA MARE !
class RegisterLoginScene
{


public:
	std::vector<User> user_vector;
	RegisterLoginScene(int type);
	~RegisterLoginScene();

	void UpdateTypeEvent(sf::RenderWindow* window, sf::Event event);
	void UpdateBtns(sf::RenderWindow* window);
	void Print(sf::RenderWindow* window);

private:
	sf::Font m_font;
	sf::RectangleShape m_background;
	sf::Font arial;
	std::shared_ptr<User> user = SingleTon<User>::GetInstance();
	int m_type;

	std::map<std::string, ButtonSFML*> m_buttons;
	std::map<std::string, Textbox*>m_textboxes;

	//Functions
	void InitButtons(sf::RenderWindow* window);
	void InitButtonsLogin(sf::RenderWindow* window);
	void InitButtonsRegister1(sf::RenderWindow* window);
	void InitButtonsRegister2(sf::RenderWindow* window);
	void InitTextboxesLogin(sf::RenderWindow* window);
	void InitTextboxesRegister1(sf::RenderWindow* window);
	void InitTextboxesRegister2(sf::RenderWindow* window);
	void InitFonts();
};

