#pragma once
#include "Interactable.h"
#include "Inventory.h"
#include "Player.h"
#include "Wall.h"
#include <array>

class Chest : public Interactable {
	private:
		// Loot generation
		std::vector<std::array<int,2>> loot;
		sf::Vector2f position;
		int pool;

		sf::Texture texture;

		sf::Vector2f position;
		sf::FloatRect detection;
		int interact;

	public:
		// Constructor
		Chest(int p, sf::Vector2f pos);

		// Generate loot based on pool
		void spawnLoot();

		// Detect player when loot is available
		void update(Player* p);

		// Handle player's input for collecting loot
		void handleInput(Input* input, Player* p, Inventory* i);
};

