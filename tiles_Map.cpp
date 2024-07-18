#include "tiles_Map.h"
#include <iostream> // Include the necessary header for std::cout

//Defualt Constructor//
tiles_Map::tiles_Map(int id, sf::Vector2f initPos) {
	// Set texture ID and load texture
	textureIndex = id;
	texture.loadFromFile(spriteSheets[textureIndex]);

	// Set tile sprite sizes
	s_Width = 16;
	s_Height = 16;

	// Get number of rows & columns in texture
	columns = texture.getSize().x / s_Width;
	rows = texture.getSize().y / s_Height;

	// Set initial position for creating tiles
	initialPos = initPos;

	// Set initial position for loading intRect objects
	left = 0;
	top = 0;

	// Resize texture vector
	tiles_Textures.resize(columns * rows);

	// Get spritesheet image from texture
	sf::Image spritesheet = texture.copyToImage();

	// Create tile textures according to number of rows and columns in image
	for (int textureLoop = 0; textureLoop < tiles_Textures.size(); ++textureLoop) {
		// Set x position of texture rect
		left = (textureLoop % columns) * 16;

		// Create intRect object to store tile texture dimensions
		sf::IntRect texturerect(left, top, s_Width, s_Height);

		// If at last column in row, move down one row in image
		if (textureLoop % columns == columns - 1) {
			top += s_Height;
		}

		// Load tile texture according to dimensions of texturerect
		tiles_Textures[textureLoop].texture.loadFromImage(spritesheet, texturerect);
	}
};


// Deconstructor
tiles_Map::~tiles_Map() {};

// Returns reference to the vector of tile pointers
std::vector<tile_Single*>& tiles_Map::getTiles() {
	return individual_Tiles;
};

// Draws tiles to window
void tiles_Map::draw(sf::RenderWindow* window, sf::View v) {

	for (auto& tile : individual_Tiles) {
		// Check if within screen x & y bounds
		visibleX = tile->getPosition().x > v.getCenter().x - screenX - 120 && tile->getPosition().x < v.getCenter().x + screenX;
		visibleY = tile->getPosition().y > v.getCenter().y - screenY - 120 && tile->getPosition().y < v.getCenter().y + screenY;

		// If within both x & y bounds
		if (visibleX && visibleY) {
			if (tile->getListPos() != -1) {
				window->draw(*tile);
			}
		}
	}
};

// loadFromCSV function
// Loads postions from the spreadsheet, and passes the attributes from the tiles_texture vector into the individual tiles vector
void tiles_Map::loadFromCSV(const std::string& filepath) {
	// Get file
	std::ifstream file(filepath);

	// Store values from file
	std::string line;

	int row = 0;

	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		std::string cell;

		int col = 0;

		// Get values from cell
		while (std::getline(lineStream, cell, ',')) {
			try {
				// Convert cell value to int
				int tileListPos = std::stoi(cell);

				// Processing only if conversion to integer succeeds
				sf::Vector2f position{ col * 100.f, row * 100.f };

				TileProperties& tile = tiles_Textures.at(tileListPos); // Use .at() for bounds checking

				// Create new tile and add to vector
				tile_Single* newTile = new tile_Single(tile.texture, position);
				individual_Tiles.push_back(newTile);

				// Set values
				newTile->setListPos(tileListPos);
			}
			catch (const std::invalid_argument& e) {
				// Handle non-integer values
				std::cerr << "Invalid argument: " << cell << " is not an integer." << std::endl;
			}
			catch (const std::out_of_range& e) {
				// Handle out-of-range values
				if (stoi(cell) != -1) {
					std::cerr << "Out of range: " << cell << " is too large for an int." << std::endl;
				}
			}
			catch (const std::exception& e) {
				// Handle other unexpected errors
				std::cerr << "Unexpected error: " << e.what() << std::endl;
			}
			col++;
		}
		row++;
	}
}

// loadCollisionCSV function
// Loads values from spreadsheet and creates appropriate collision object for each tile
void tiles_Map::loadCollisionsCSV(const std::string& filepath) {
	// Get file
	std::ifstream file(filepath);

	// Store values from file
	std::string line;

	int row = 0;
	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		std::string cell;

		int col = 0;

		// Get values from cell
		while (std::getline(lineStream, cell, ',')) {
			try {
				// Convert cell value to int
				int tileListPos = std::stoi(cell);

				// Processing only if conversion to integer succeeds
				sf::Vector2f position{ initialPos.x + col * 100.f, initialPos.y + row * 100.f };

				// Create collision object based on value in cell
				if (tileListPos < 4) {
					collisions.push_back(Diagonal(position, { collisionTypes[tileListPos][0].x + position.x, collisionTypes[tileListPos][0].y + position.y }, { collisionTypes[tileListPos][1].x + position.x, collisionTypes[tileListPos][1].y + position.y }));
				}
				else {
					collisions.push_back(Wall(position, { collisionTypes[tileListPos][0].x, collisionTypes[tileListPos][0].y }, { collisionTypes[tileListPos][1].x, collisionTypes[tileListPos][1].y }));
				}
			}
			catch (const std::invalid_argument& e) {
				// Handle non-integer values
				std::cerr << "Invalid argument: " << cell << " is not an integer." << std::endl;
			}
			catch (const std::out_of_range& e) {
				// Handle out-of-range values
				std::cerr << "Out of range: " << cell << " is too large for an int." << std::endl;
			}
			catch (const std::exception& e) {
				// Handle other unexpected errors
				std::cerr << "Unexpected error: " << e.what() << std::endl;
			}
			col++;
		}
		row++;
	}
}

// Update function
// Loops through each collision object's update function
void tiles_Map::update(Player* p, sf::View v, float dt) {
	for (auto& collision : collisions) {
		if (typeid(collision) == typeid(Diagonal)) {
			std::get<Diagonal>(collision).update(p, v, dt);
		}
		else {
			std::get<Wall>(collision).update(p, v, dt);
		}
	}
}

void tiles_Map::update(std::vector<Chaser>* c, sf::View v, float dt) {
	for (auto& collision : collisions) {
		for (int i = 0; i < c->size(); i++) {
			std::get<Wall>(collision).update(&(c->at(i)), v, dt);
		}
	}
}

// Set Border function - sets the darkness of a tile according to its distance from the bounds rectangle, creating a gradient border
void tiles_Map::setBorder(sf::RectangleShape bounds) {
	// Store color values according to x & y positions of tile
	double colorX;
	double colorY;


	for (auto& tile : individual_Tiles) {

		// If tile is to the left of the specified bounds, store distance from left of bounds as color value
		if (tile->getPosition().x <= bounds.getPosition().x) {
			colorX = 255 - abs(tile->getPosition().x - bounds.getPosition().x) / 5;
			// If tile is to the right of the specified bounds, store distance from right of bounds as color value
		}
		else if (tile->getPosition().x >= bounds.getPosition().x + bounds.getSize().x) {
			colorX = 255 - abs(tile->getPosition().x - bounds.getPosition().x - bounds.getSize().x) / 5 - 20;
			// If within bounds, set colorX to 255
		}
		else {
			colorX = 255;
		}

		// If tile is above the specified bounds, store distance from top of bounds as color value
		if (tile->getPosition().y <= bounds.getPosition().y) {
			colorY = 255 - abs(tile->getPosition().y - bounds.getPosition().y) / 4;
			// If tile is below the specified bounds, store distance from bottom of bounds as color value
		}
		else if (tile->getPosition().y >= bounds.getPosition().y + bounds.getSize().y) {
			colorY = 255 - abs(tile->getPosition().y - bounds.getPosition().y - bounds.getSize().y) / 4 - 25;
			// If within bounds, set colorY to 255
		}
		else {
			colorY = 255;
		}

		// If colorX is negative, reset to 0
		if (colorX < 0) {
			colorX = 0;
		}

		// If colorY is negative, reset to 0
		if (colorY < 0) {
			colorY = 0;
		}

		// If tile is within x bounds but outside y bounds, set darkness according to colorY
		if (tile->getPosition().x >= bounds.getPosition().x && tile->getPosition().x <= bounds.getPosition().x + bounds.getSize().x - 100 &&
			(tile->getPosition().y < bounds.getPosition().y || tile->getPosition().y >= bounds.getPosition().y + bounds.getSize().y)) {

			// Set color
			tile->setFillColor(sf::Color(colorY, colorY, colorY, 255 - colorY / 8));

			// If tile is within y bounds but outside x bounds, set darkness according to colorX
		}
		else if (tile->getPosition().y >= bounds.getPosition().y && tile->getPosition().y <= bounds.getPosition().y + bounds.getSize().y - 100 &&
			(tile->getPosition().x < bounds.getPosition().x || tile->getPosition().x >= bounds.getPosition().x + bounds.getSize().x)) {

			// Set color
			tile->setFillColor(sf::Color(colorX, colorX, colorX, 255 - colorX / 8));

			// Otherwise, set darkness equal to colorX + colorY if both values do not equal 255
		}
		else if (colorX != 255 && colorY != 255) {

			// Set both color values to 0 if their sum is 255 or greater
			if (colorX + colorY <= 255) {
				colorX = 0;
				colorY = 0;
			}

			// Set color
			tile->setFillColor(sf::Color(colorX + colorY, colorX + colorY, colorX + colorY, 255 - (colorX + colorY) / 16));
			// Set color to white otherwise
		}
		else {
			tile->setFillColor(sf::Color::White);
		}
	}
}