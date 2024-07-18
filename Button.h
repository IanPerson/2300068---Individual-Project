// Parent class for types of buttons
// Potential child classes for button:
// - Menu traversal
// - Preferences(?)
// - Pause button
#pragma once
#include "Interactable.h"
#include "Cursor.h"

// Forward declaration for necessary classes
class Save;
class Level;


class Button : public Interactable {
	private:
		// Get screen resolution
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Text & font for button description
		sf::Text description;
		sf::Font font;

		// Determines function of button
		int buttonID;

	public:
		// Constructors and deconstructors
		Button();
		Button(sf::Vector2f size, sf::Vector2f pos, int id);
		~Button();

		// Update function
		// The cursor's collision box intersects the button, enable mouse interaction
		// Otherwise, disable mouse interaction
		void update(Cursor m);

		// HandleInput function
		// Highlight the button if the mouse is hovering over it
		// Execute a specific function according to the button's id when clicked
		void handleInput(bool* isPaused, int* menu, Save* save, Save* save2, Level* l);

		// Draw function
		// Draws button and description to specified window
		void draw(sf::RenderWindow* w);

		// setDescription function
		// Set the text for the button to display
		void setDescription(std::string text);
};

