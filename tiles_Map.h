#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/GameObject.h"
#include "tile_Single.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Diagonal.h"
#include "Wall.h"
#include <variant>

class tiles_Map : public GameObject
{
public:
	//Defualt Constructor//
	tiles_Map(int id, sf::Vector2f initPos);

	//Deocnsturctor//
	~tiles_Map();

	// returns reference to the vecotr of tile pointers//
	std::vector<tile_Single*>& getTiles();

	// Draws tiles to window//
	void draw(sf::RenderWindow* window, sf::View v);

	// loadFromCSV function
	// Loads postions from the spreadsheet, and passes the attributes from the tiles_texture vector into the individual tiles vector
	void loadFromCSV(const std::string& filepath);

	// loadCollisionCSV function
	// Loads values from spreadsheet and creates appropriate collision object for each tile
	void loadCollisionsCSV(const std::string& filepath);

	// Update function
	// Loops through each collision object's update function
	void update(Player* p, sf::View v, float dt);
	void update(std::vector<Chaser>* c, sf::View v, float dt);

	// setBorder function
	// Creates a gradient around the specified bounds by setting the darkness of a tile according to its distance from the bounds
	void setBorder(sf::RectangleShape bounds);

private:
	// Get screen resolution
	int screenX = (int)sf::VideoMode::getDesktopMode().width * 0.8 / 2;
	int screenY = (int)sf::VideoMode::getDesktopMode().height * 0.8 / 2;

	bool visibleX;
	bool visibleY;

	//Struct to hold the tile properties//
	struct TileProperties
	{
		//tile texture//
		sf::Texture texture;
		//bool to check if tile is door or gate//
		bool doorGate;
	};

	// texture is map texture//
	sf::Texture texture;

	//vector of the tile properties struct for individual tiles//
	std::vector<TileProperties> tiles_Textures;
	//vector for the tile_Single class//
	std::vector<tile_Single*> individual_Tiles;

	//arrayPos variable//
	int arrayPos;

	// Stores index for accessing texture
	int textureIndex;

	// Vector for collision objects
	std::vector<std::variant<Wall, Diagonal>> collisions;

	// Presets for collision types
	sf::Vector2f collisionTypes[14][2] = {

		// Diagonal Collisions
		// 1st Vector -> Starting point for creating collision
		// 2nd Vector -> End point for creating collision

		// Push up and left
		{ {0, 100}, {100, 0} },

		// Push down and right
		{ {100, 0}, {0, 100} },

		// Push down and left
		{ {0, 0}, {100, 100} },

		// Push up and right 
		{ {100, 100}, {0, 0} },


		// Square Collisions
		// 1st vector -> Offset from tile position
		// 2nd vector -> Size of collision

		// No collision
		{ {0,0}, {0,0} },

		// Full
		{ {0,0}, {100,100} },

		// Half Vertical
		{ {0,0}, {50,100} },
		{ {50,0}, {50,100} },
		
		// Half Horizontal
		{ {0,50}, {100,50} },
		{ {0,0}, {100,50} },
		
		// Corners
		{ {0,0}, {50,50} },
		{ {0,50}, {50,50} },
		{ {50,0}, {50,50} },
		{ {50,50}, {50,50} },

		// Add any extras collision types below

	};

	// Starting position when loading tile map
	sf::Vector2f initialPos;

	//Sprite width//
	int s_Width;
	//Sprite height//
	int s_Height;

	// Determines size of tile map vectors 
	int columns;
	int rows;

	// Used for loading textures
	int left;
	int top;

	// File locations for tile spritesheets
	std::string spriteSheets[8] = { "gfx/Tilesets/Grass.png", "gfx/Tilesets/Hills.png", "gfx/Tilesets/Planted_Dirt.png", "gfx/Tilesets/Tilled_Dirt.png", "gfx/Tilesets/Plant_Dirt.png","gfx/Tilesets/WoodenHouse.png", "gfx/Tilesets/Fences.png", "gfx/Tilesets/Water.png"};

};

