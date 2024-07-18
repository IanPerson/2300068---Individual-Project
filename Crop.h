#pragma once
#include <string>
#include "Interactable.h"
#include "Inventory.h"

class Crop : public Interactable {
	private:
		// Determines the plant currently growing
		int cropID;

		// Variables for crop growth
		double cropGrowthLevel = 0;
		float cropGrowthRate = 0;

		// Determines if crop has been planted and if it's ready to harvest
		bool cropHarvestReady;
		bool isPlanted;
		
		int interact = 1;

		// Animations
		Animation planted;
		Animation growing;
		Animation ready;
		Animation* currentAnimation;

		// Textures for animations
		sf::Texture wheat;
		sf::Texture potato;
		sf::Texture carrot;
		sf::Texture onion;
		sf::RectangleShape plantSprite;

	public:
		// Constructor
		Crop(float x, float y);

		// Functions

		// plantCrop function
		// Starts growing process according to the given ID
		void plantCrop(int id, Inventory* inv);

		// harvestCrop function
		// Adds seeds & food to inventory and resets crop
		void harvestCrop(Inventory* inv);

		// cropGrowth function
		// Increases growth level over time and updates texture depending on growth level
		void cropGrowth();

		// handleInput function
		// Plant or harvest crop when space is pressed
		void handleInput(Player p, Input* i, Inventory* inv);

		// Update function
		// Check collision with player & display growth progress
		void update(Player* p, float dt);

		// getPlant function
		// Returns plant object
		sf::RectangleShape getPlant();

		// getPlanted
		// Returns value of isPlanted
		bool getPlanted();
};

