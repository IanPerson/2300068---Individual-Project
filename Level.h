#pragma once

#include <SFML/Graphics.hpp>
#include "Framework/BaseLevel.h"
#include "Framework/AudioManager.h"
#include "Vector"
#include "Save.h"
#include "Menu.h"
#include "DayCycle.h"
#include "EnemyManager.h"
#include "House.h"

class Level : BaseLevel {
public:
	Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud);
	~Level();

	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:
	// Default functions for rendering to the screen.
	

	// Default variables for level class.

	// Vectors for objects
	std::vector<Save*> saves;
	std::vector<Room*> rooms;
	std::vector<Field> fields;
	std::vector<Menu*> menus;
	std::vector<Animal> animals;
	std::vector<House*> houses;

	// Current indexes for rooms and menu vectors, determines what is drawn and updated
	int currentRoom;
	int currentMenu;

	// States for pausing, trading, and using the inventory
	bool isPaused;
	bool currentlyTrading;
	bool inInventory;

	// Create controllable objects
	Player player;
	Cursor mouse;

	// Create inventory
	Inventory inventory{ 5, 5 };

	// Create family
	Family family{ 1500, 1500 };

	// Create vendor
	Vendor shopkeeper{2}; 

	sf::RectangleShape test1;

	DayCycle cycle;

	// Create rooms
	Room* farm = new Room{ {8000, 8000} };
	House home{ {1500,1200}, 1};

	House house1{ {2800,3200}, 2 };
	House house2{ {2800,4600}, 3 };
	House house3{ {4800,3200}, 4 };
	House house4{ {5700,3200}, 5 };
	House house5{ {4800,5600}, 6 };
	House house6{ {4900,7100}, 7 };

	EnemyManager spawner{ {8000,8000},{1100,1000} };

	// Create fields
	Field field1{ 2800, 5500, 5, 9 };
	Field field2{ 3400, 3200, 7, 4 };
	Field field3{ 3600, 1200, 5, 7 };
	Field field4{ 4800, 1200, 5, 7 };
	Field field5{ 4700, 6600, 4, 9 };

	// Text for frame rate - testing only
	sf::Text frameRate;
	sf::Font font;

	// Moving view - for player & room
	sf::View camera;

	// Static view - for UI
	sf::View ui;
};