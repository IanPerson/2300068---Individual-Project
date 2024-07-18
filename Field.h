#pragma once
#include "Interactable.h"
#include "Crop.h"
#include <vector>

class Field : public Interactable {
	private:
		// Get screen resolution
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Vector to store crop objects
		std::vector<Crop> allCrops;

		// # of rows & columns of crops in field
		int rows;
		int columns;

		// Texture for crops
		sf::Texture cropTexture;

	public:
		// Constructors and deconstructor
		Field();
		Field(float x, float y, int rows, int columns);
		~Field();

		// handleInput function
		// Calls input functions for all crops within a certain distance of player
		void handleInput(Player p, Input* i, sf::View v, Inventory* inv);

		// Update function
		// Calls update functions for all crops within a certain distance of player
		void update(Player* p, sf::View v, float dt);

		// drawCrops function
		// Draws crop object in specified window if object is within boundaries of window
		void drawCrops(sf::View v, sf::RenderWindow* w);
};

