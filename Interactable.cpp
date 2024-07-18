#include "Interactable.h"
#include <iostream>

// Default constructor
Interactable::Interactable() {
	// Disable interaction
	keyboardInteract = false;
	mouseInteract = false;

	// Set properties
	setSize({ 50, 50 });
	setFillColor(sf::Color::Green);
	setCollisionBox(collisionBox);

	// Set collision box
	collisionBox.width = getSize().x + 100;
	collisionBox.height = getSize().x + 100;
	setCollisionBox(collisionBox);
}

// Overloaded constructor
// x -> x position
// y -> y position
Interactable::Interactable(float x, float y) {
	// Disable interaction
	keyboardInteract = false;
	mouseInteract = false;

	// Set properties
	setSize({ 50, 50 });
	setPosition({ x, y });
	setFillColor(sf::Color::Green);

	// Set collision box
	collisionBox.width = getSize().x + 100;
	collisionBox.height = getSize().x + 100;
	setCollisionBox(collisionBox);
}

// Destructor
Interactable::~Interactable() {

}

// Update Function -
// If the player is within range of the object's collision box, the object is highlighted and interaction is enabled
void Interactable::update(Player p) {
	// If player's collision box is intersecting the object's collision box, set the object's color to red and enable keyboard interaction
	// Otherwise, set the object's color to green and disable keyboard interaction
	if (p.getCollisionBox().intersects(collisionBox)) {
		setFillColor(sf::Color::Red);
		keyboardInteract = true;
	}
	else {
		setFillColor(sf::Color::Green);
		keyboardInteract = false;
	}
}

// Handle Function -
// If interaction is enabled and space is pressed, a test interact() function executes
void Interactable::handleInput(float dt) {
	if (keyboardInteract && input->isPressed(sf::Keyboard::F)) {
		interact();
	}
}

// interactCheck -
// Returns the value of specified interaction variable
bool Interactable::interactCheck(int type) {
	switch (type) {
		case 0:
			return keyboardInteract;
			break;
		case 1:
			return mouseInteract;
			break;
	}

	return false;
}

// Test function for interaction
void Interactable::interact() {
	std::cout << "Interacted";
}