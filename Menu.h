#pragma once
#include "Framework/GameObject.h"
#include "Button.h"
#include "Cursor.h"
#include <vector>

class Menu : public GameObject {
	private:
		// Get screen resolution
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Vectors for buttons and text
		std::vector<Button*> buttons;
		std::vector<sf::Text*> allText;

		// Font
		sf::Font font;

	public:
		// Constructors
		Menu();
		Menu(int id);
		
		// addButton function
		// Adds a button to the buttons vector using the size, position, and ID parameters
		void addButton(sf::Vector2f size, sf::Vector2f position, int buttonID);

		// handleInput function
		// Calls input functions for all buttons in menu
		void handleInput(bool* isPaused, int* menu, Save* save, Save* save2, Level* l);

		// Update function
		// Calls update functions for all buttons in menu
		void update(Cursor m);

		// Draw function
		// Draws all elements of menu in specified window
		void draw(sf::RenderWindow* w);

		// setInput function
		// Sets input for all buttons in menu
		void setInput(Input* i);
};

