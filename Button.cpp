#include "Button.h"
#include "Cursor.h"
#include "Save.h"

// Default Constructor
Button::Button() {
	// Default ID
	buttonID = 0;


	// Set color
	setFillColor(sf::Color(200, 160, 140, 255));
	setOutlineThickness(screenX / 200);
	setOutlineColor(sf::Color(140, 100, 80, 255));

	// Set default size
	setSize({ screenX / 50.f, screenX / 50.f });
	// Set position, offset by outline
	setPosition(getOutlineThickness(), getOutlineThickness());

	// Set collision
	collisionBox.height = getSize().y * 2;
	collisionBox.width = getSize().x * 2;
	collisionBox.left = getPosition().x;
	collisionBox.top = getPosition().y;
	setCollisionBox(collisionBox);

	// Load font
	font.loadFromFile("font/Daydream.ttf");

	// Set description properties
	description.setFont(font);
	description.setCharacterSize(20);
	description.setFillColor(sf::Color::White);
	description.setOutlineColor(sf::Color::Black);
	description.setOutlineThickness(5);
}

// Overloaded Constructor
// size - button size
// pos - initial position
// id - function of button
Button::Button(sf::Vector2f size, sf::Vector2f pos, int id) {
	// Set ID
	buttonID = id;

	// Set color
	setFillColor(sf::Color(200, 160, 140, 255));
	setOutlineThickness(screenX / 200);
	setOutlineColor(sf::Color(140, 100, 80, 255));

	// Set size & initial position
	setSize({ screenX / size.x, screenX / size.y });
	setPosition(pos.x + getOutlineThickness(), pos.y + getOutlineThickness());

	// Set collision
	collisionBox.height = getSize().y * 2;
	collisionBox.width = getSize().x * 2;
	collisionBox.left = getPosition().x;
	collisionBox.top = getPosition().y;
	setCollisionBox(collisionBox);

	// Load font
	font.loadFromFile("font/Daydream.ttf");

	// Set description properties
	description.setFont(font);
	description.setCharacterSize(screenX / 150);
	description.setFillColor(sf::Color(180, 140, 120, 255));
	description.setOutlineColor(sf::Color(140, 100, 80, 255));
	description.setOutlineThickness(5);

	// Set description text according to ID
	switch (buttonID) {
		case 0:
			// Unpause game
			description.setString("Resume");
			break;
		case 1:
			// Return to main menu
			description.setString("Main Menu");
			break;
		case 2:
			// Return to pause menu
			description.setString("    Back to\nPause Menu");
			break;
		case 3:
		case 4:
			// See controls
			description.setString("Controls");
			break;
		case 5:
		case 6:
			// See game instructions
			description.setString("How to Play");
			break;
		case 7:
			// Save checkpoint
			description.setString("Save");
			break;
		case 8:
			// Load checkpoint
			description.setString("Load");
			break;
		case 9:
			// Exit game
			description.setString("Exit");
			break;
	}

	// Position description in center of button
	description.setPosition(getPosition().x + getSize().x / 2 - description.getGlobalBounds().width / 2, getPosition().y + getSize().y / 2 - description.getGlobalBounds().height / 2);
}

// Deconstructor
Button::~Button() {

}


// Update function - 
// If the cursor collides with the button, enable mouse interaction
// Otherwise, disable mouse interaction
void Button::update(Cursor m) {
	if (m.getCollisionBox().intersects(getCollisionBox())) {
		mouseInteract = true;
	} else {
		mouseInteract = false;
	} 
}

// HandleInput function - 
// Highlight the button if the mouse is hovering over it
// Execute a specific function according to the button's id when clicked
void Button::handleInput(bool* isPaused, int* menu, Save* save, Save* save2, Level* l) {

	// Check if isPaused is initialized before detecting input
	if (isPaused != NULL) {
		// If player clicks on button, reset color and activate button according to ID
		if (input->isLeftMousePressed() && mouseInteract) {
			// Reset colors
			setFillColor(sf::Color(200, 160, 140, 255));
			description.setFillColor(sf::Color(240, 200, 180, 255));
			description.setOutlineColor(sf::Color(160, 120, 100, 255));
			
			// ID determines what to do
			switch (buttonID) {
				// Pause/Unpause
				case 0:
					if (!*isPaused) {
						*isPaused = true;
					} else {
						*isPaused = false;
						*menu = 1;
					}
					break;
				// Reset game and return to main menu
				case 1:
					save->resetTo(0, l);
					*menu = 0;
					break;
				// Move to pause menu
				case 2:
					*menu = 1;
					break;
				// Move to controls (main menu)
				case 3:
					*menu = 2;
					break;
				// Move to controls (pause menu)
				case 4:
					*menu = 4;
					break;
				// Move to instructions (main menu)
				case 5:
					*menu = 3;
					break;
				// Move to instructions (pause menu)
				case 6:
					*menu = 5;
					break;
				// Save checkpoint and unpause
				case 7:
					*isPaused = false;
					*menu = 1;
					save2->replaceSave(0, *l);
					break;
				// Load checkpoint
				case 8:
					save2->resetTo(0, l);
					break;
				// Exit game
				case 9:
					exit(0);
			}

		// Highlight button when cursor is colliding with it
		} else if (mouseInteract) {
			setFillColor(sf::Color(180, 140, 120, 255));
			description.setFillColor(sf::Color(220, 180, 160, 255));
			description.setOutlineColor(sf::Color(140, 100, 80, 255));
		// Reset colors when cursor is not colliding with button
		} else {
			setFillColor(sf::Color(200, 160, 140, 255));
			description.setFillColor(sf::Color(240, 200, 180, 255));
			description.setOutlineColor(sf::Color(160, 120, 100, 255));
		}
	}
}

// Draw function
// Draws button and description to specified window
void Button::draw(sf::RenderWindow* w) {
	w->draw(*this);
	w->draw(description);
}

// setDescription function
// Set the text for the button to display
void Button::setDescription(std::string text) {
	description.setString(text);
	// Position description in center of button
	description.setPosition(getPosition().x + getSize().x / 2 - description.getGlobalBounds().width / 2, getPosition().y + getSize().y / 2 - description.getGlobalBounds().height / 2);
}