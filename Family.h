#pragma once
#include "Interactable.h"
#include "Inventory.h"
#include <iostream>

class Family : public Interactable {
private:
	// Tracks the object's hunger level
	int food;

	// Time remaining until hunger level decreases
	float foodCountdown;

	// Text for displaying current hunger level
	sf::Text foodCounter;
	sf::Font font;

	int interact = 1;

	// Texture and animation
	Animation idle;
	sf::Texture familyIdle;

public:
	// Constructor and Deconstructor
	Family(float x, float y);
	~Family();

	// Update function - See cpp file for details
	// Player p -> Check collision with object
	// float dt -> used for the hunger count down
	// Inventory* inventory -> check if food is in checked inventory when feeding family
	void update(Player* p, float dt, Inventory* inventory, int* menu, bool* isPaused);

	// Returns foodCounter variable
	sf::Text getText();
};
