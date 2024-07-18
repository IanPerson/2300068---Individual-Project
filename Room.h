#pragma once

#include "Framework/GameObject.h"
#include "Framework/Input.h"

#include <String>

#include "Door.h"
#include "Button.h"
#include "Field.h"
#include "Vendor.h"
#include "Animal.h"
#include "Family.h"
#include "tiles_Map.h"
#include "Lantern.h"
#include "Chest.h"

class Room : public GameObject {
	private:
		
		// Determines area where player can move
		sf::RectangleShape playerArea;

		std::vector<tiles_Map*> tileMaps;

		// Textures
		sf::Texture lanternTexture;
		sf::Texture roomTexture;
		sf::Texture chestTexture;


		// Vector variable containing all current door objects
		// Can create and destroy door objects with few issues
		std::vector<Door> allDoors;

		std::vector<Vendor> vendors;
		std::vector<Field> fields;
		std::vector<Animal> animals;
		std::vector<Lantern*> lanterns;
		std::vector<Chest*> chests;

		std::vector<Wall> decorations;


	public:
		// Constructor
		Room(sf::Vector2f areaSize);

		// FUNCTIONS

		// Get traversable area
		sf::RectangleShape getArea();
		std::vector<Chest*> getChestList();

		// Draws all objects in room
		void draw(sf::RenderWindow* w, sf::View v);
		void drawLanterns(sf::RenderWindow* w);

		// update() - see room.cpp for info
		void update(Player *player, sf::View v, float dt);

		// Get input for doors
		void handleInput(Input* input, int* currentRoom, Player* player, Inventory* inv);

		// addDoor() - see room.cpp for info
		void addDoor(sf::Vector2f position, sf::Vector2f moveCoords, float rotation, int moveTo);
		void addChest(int pool, sf::Vector2f position);
		void addLantern(sf::Vector2f position);

		// loadTexture() - see room.cpp for info
		void loadTexture(sf::Texture texture);

		// addTileMap() - see room.cpp for info
		void addTileMap(int id, const std::string& map);
		void addTileMap(int id, const std::string& map, const std::string& collision);

		std::vector<Lantern*> getLanterns();
};