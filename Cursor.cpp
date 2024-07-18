#include "Cursor.h"

// Constructor
Cursor::Cursor() {

	// Set size & collision box
	setSize({ sf::VideoMode::getDesktopMode().width / 125.f, sf::VideoMode::getDesktopMode().height / 45.f});

	collisionBox.width = 5;
	collisionBox.height = 5;

	// Load sprite
	texture.loadFromFile("gfx/cursor.png");
	setTexture(&texture);
}

// Deconstructor
Cursor::~Cursor() {

}

// Update function -
// Move the object according to the position of the mouse on screen & update collision box
void Cursor::update(Input i) {
	// Set position of object
	setPosition(i.getMouseX(), i.getMouseY());
	
	// Update collision box position
	collisionBox.left = i.getMouseX();
	collisionBox.top = i.getMouseY();
}