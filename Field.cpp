#include "Field.h"

// Default contructor
Field::Field() {
	// Default position
	setPosition(sf::Vector2f(100,100));

	// Default rows and columns
	rows = 6;
	columns = 10;

	// Set size according to rows * columns
	setSize({ columns * 100.f, rows * 100.f });

	// Load texture
	cropTexture.loadFromFile("gfx/Crop.png");

	// For each row and column, create a crop object and push it into the crops vector
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Crop crop(getPosition().x + 100 * j + 5, getPosition().y + 100 * i + 5);
			crop.setTexture(&cropTexture);
			allCrops.push_back(crop);
		}
	}

}

// Overloaded constructor
Field::Field(float x, float y, int r, int c) {
	// Set position
	setPosition({ x, y });

	// Set rows and columns
	rows = r;
	columns = c;

	// Set size according to rows * columns
	setSize({ columns * 100.f, rows * 100.f });

	// Load texture
	cropTexture.loadFromFile("gfx/Crop.png");

	// For each row and column, create a crop object and push it into the crops vector
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Crop crop(getPosition().x + 100 * j, getPosition().y + 100 * i );
			crop.setTexture(&cropTexture);
			allCrops.push_back(crop);
		}
	}
}

// Deconstructor
Field::~Field() {

}

// handleInput function
// Calls input functions for all crops within a certain distance of player
void Field::handleInput(Player p, Input* input, sf::View v, Inventory* inv) {
	for (int i = 0; i < allCrops.size(); i++) {
		// If crop is within range on screen, call input function
		if ((allCrops[i].getPosition().x > v.getCenter().x - screenX / 8 && allCrops[i].getPosition().x < v.getCenter().x + screenX / 8) &&
			(allCrops[i].getPosition().y > v.getCenter().y - screenX / 8 && allCrops[i].getPosition().y < v.getCenter().y + screenX / 8)) {

			allCrops[i].handleInput(p, input, inv);

		}
	}
}

// Update function
// Calls update functions for all crops within a certain distance of player
void Field::update(Player* p, sf::View v, float dt) {

	for (int i = 0; i < allCrops.size(); i++) {
		// If crop is within range on screen, call update function
		if ((allCrops[i].getPosition().x > v.getCenter().x - screenX / 5 && allCrops[i].getPosition().x < v.getCenter().x + screenX / 5) &&
			(allCrops[i].getPosition().y > v.getCenter().y - screenX / 5 && allCrops[i].getPosition().y < v.getCenter().y + screenX / 5)) {

			allCrops[i].update(p, dt);

		}
		
		// If something is planted in this crop tile, call cropGrowth function
		if (allCrops[i].getPlanted()) {
			allCrops[i].cropGrowth();
		}
	}
}

// drawCrops function
// Draws crop object in specified window if object is within boundaries of window
void Field::drawCrops(sf::View v, sf::RenderWindow* w) {

	for (int i = 0; i < allCrops.size(); i++) {
		// If crop is within the bounds of the window, draw crop
		if ((allCrops[i].getPosition().x > v.getCenter().x - v.getSize().x / 2 - 100 && allCrops[i].getPosition().x < v.getCenter().x + v.getSize().x / 2) && 
			(allCrops[i].getPosition().y > v.getCenter().y - v.getSize().y / 2 - 100 && allCrops[i].getPosition().y < v.getCenter().y + v.getSize().y / 2)) {

				w->draw(allCrops[i]);

				// If something is planted in crop tile, draw plant
				if (allCrops[i].getPlanted()) {
					w->draw(allCrops[i].getPlant());
				}
		}
	}
}