#include "Door.h"

// Constructor
Door::Door() {
	setSize({ 100, 50 });
	setPosition({ 0, 0 });

	collisionBox.width = getSize().x + 100;
	collisionBox.height = getSize().x + 120;

	// Set position and room that the door will teleport player to
	moveCoords = { 0, 0 };
	moveTo = 0;
}

// Overloaded Constructor
// Position - X & Y position of object
// Move - Location to teleport player to when interacted with
// Rotation - Rotation of object (should only be a multiple of 90)
// Room - Which room the door will move the player to
Door::Door(sf::Vector2f position, sf::Vector2f move, float rotation, int room) {

	// Set door properties
	setSize({ 100, 50 });
	setPosition(position);
	setRotation(rotation);

	// Set position and room that the door will teleport player to
	moveCoords = move;
	moveTo = room;

	// Update collision box size and position
	collisionBox.width = getSize().x + 100;
	collisionBox.height = getSize().x + 100;

	// Update collision box based on rotation
	if (rotation == 90) {
		collisionBox.left =  -100;
		collisionBox.top = 0;
	}
	else if (rotation == 180) {
		collisionBox.left = -150;
		collisionBox.top = -100;
	}
	else if (rotation == 270) {
		collisionBox.left = -100;
		collisionBox.top = -100;
	}
	else {
		collisionBox.left = -50;
		collisionBox.top = -100;
	}

	setCollisionBox(collisionBox);
}

// Deconstructor
Door::~Door() {
	
}

// Update function - 
// Highlights door red and enables keyboard interaction if player is nearby, otherwise the door is green and cannot be interacted with
// Adjusts position of collision box depending on the rotation of the door
void Door::update(Player* p) {

	// Collision check
	if (p->getCollisionBox().intersects(getCollisionBox())) {
		setFillColor(sf::Color(255,0,0,50));
		keyboardInteract = true;
		if (interact == 1) {
			p->setPrompt("F to enter");
			p->setInteract(p->getInteract() + 1);
			interact -= 1;
		}
	} else {
		setFillColor(sf::Color(0,255,0,50));
		keyboardInteract = false;
		if (interact == 0) {
			p->setPrompt("");
			p->setInteract(p->getInteract() - 1);
			interact += 1;
		}
	}
}

// Handle input function - 
// If the player presses the spacebar while in range of the door, updates the current room depending on the value of moveTo
// Sets the player's position according to moveCoords
void Door::handleInput(Input* input, int* currentRoom, Player* player) {
	if (keyboardInteract && input->isPressed(sf::Keyboard::F)) {
		input->setKeyUp(sf::Keyboard::F);
		*currentRoom = moveTo;
		player->setPosition(moveCoords);
		player->setInteract(player->getInteract() - 1);
		interact += 1;
	}
}

void Door::setMoveTo(int m) {
	moveTo = m;
}

void Door::setMoveCoords(sf::Vector2f coords) {
	moveCoords = coords;
}