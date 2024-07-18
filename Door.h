#pragma once
#include "Interactable.h"

class Door : public Interactable {
	private:
		// Variables

		// Room to move to when interacted with
		int moveTo;

		// Position to teleport player to when interacted with
		sf::Vector2f moveCoords;

		int interact = 1;

	public:
		// Constructors and deconstructor
		Door();
		Door(sf::Vector2f position, sf::Vector2f move, float rotation, int room);
		~Door();

		// Functions

		// Update function - 
		// Highlights door red and enables keyboard interaction if player is nearby, otherwise the door is green and cannot be interacted with
		// Adjusts position of collision box depending on the rotation of the door
		void update(Player* p);

		// Handle input function - 
		// If the player presses the spacebar while in range of the door, updates the current room depending on the value of moveTo
		// Sets the player's position according to moveCoords
		void handleInput(Input* input, int* currentRoom, Player* player);

		void setMoveTo(int m);
		void setMoveCoords(sf::Vector2f coords);
};