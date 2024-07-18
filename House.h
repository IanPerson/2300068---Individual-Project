#pragma once
#include "Framework/GameObject.h"
#include "Wall.h"
#include "Door.h"
#include "Room.h"

class House : public GameObject {
	private:
		// Room object to enter
		Room* inside;
		
		// Door to enter room
		Door entrance;

		// Texture
		sf::Texture texture;

	public:
		// Constructor
		House(sf::Vector2f pos, int doorNum);

		// Detect player
		void update(Player* p);
		void handleInput(Input* i, int* currentRoom, Player* p);
		Room* getRoom();
};

