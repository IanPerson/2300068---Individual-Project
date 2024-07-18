#pragma once
#include "Interactable.h"
#include "Inventory.h"
#include <iostream>

class Animal : public Interactable
{
private:
	// Variables

	// Movement Variables
	sf::Vector2f position;
	sf::Vector2f direction;
	int speed;

	sf::Vector2f boundaryPos;
	sf::Vector2f boundarySize;

	int interact = 1;

	// Cooldown for collecting items
	float harvestCooldown;

	// Animations
	Animation idle;
	Animation walk;
	Animation* currentAnimation;

	// Vector for animal textures
	std::vector<sf::Texture*> animalTextures;

public:
	// Constructor and Deconstructor
	Animal(sf::Vector2f boundsPos, sf::Vector2f boundsSize);
	~Animal();

	// Functions

	// randomMovement function
	// Returns a random number which determines the direction to move
	int randomMovement();

	// Movement function
	// Moves object depending on randomMovement
	void movement(float dt);

	// Update function
	// Moves object and checks collisions
	void update(Player* p, float dt);

	void countdown(float dt);

	// handleInput function
	// Detect player input
	void handleInput(Input* input, Player* p, Inventory* inv);
};

