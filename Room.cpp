 #include "Room.h"

// Constructor
// AreaSize - Dimensions of area player can move in
Room::Room(sf::Vector2f areaSize) {
	
	// Room properties
	setPosition({ 0, 0 });
	setSize({ areaSize.x + 2200, areaSize.y + 2000 });

	// Moveable area properties
	playerArea.setSize(areaSize);
	playerArea.setPosition({ 1100, 1000 });

	lanternTexture.loadFromFile("gfx/Lantern.png");
	chestTexture.loadFromFile("gfx/Crate.png");

	lanterns.push_back(new Lantern({800, 800}, {0,0}));
	lanterns.back()->setFillColor(sf::Color(230, 230, 180, 100));
	lanterns.back()->setTexture(&lanternTexture);
	lanterns.back()->setActive(true);
}

// Add Door function - adds a door to the room according to parameters
void Room::addDoor(sf::Vector2f position, sf::Vector2f moveCoords, float rotation, int moveTo) {
	Door door{ position, moveCoords, rotation, moveTo };
	allDoors.push_back(door);
}

void Room::addChest(int pool, sf::Vector2f position) {
	Chest* chest = new Chest(pool, position);
	chest->setTexture(&chestTexture);
	chest->spawnLoot();
	chests.push_back(chest);
}

void Room::addLantern(sf::Vector2f position) {
	lanterns.push_back(new Lantern({ 600,600 }, position));
	lanterns.back()->setFillColor(sf::Color(230, 230, 180, 200));
	lanterns.back()->setTexture(&lanternTexture);
}

// Add Tile Map functions - adds a tile map to the room, and loads the map's corresponding CSV file
void Room::addTileMap(int id, const std::string& map) {
	tiles_Map* newMap = new tiles_Map(id, playerArea.getPosition());
	newMap->loadFromCSV(map);
	newMap->setBorder(playerArea);
	tileMaps.push_back(newMap);
}


// Overload - also loads a csv file for collision detection
void Room::addTileMap(int id, const std::string& map, const std::string& collision) {
	tiles_Map* newMap = new tiles_Map(id, playerArea.getPosition());
	newMap->loadFromCSV(map);
	newMap->loadCollisionsCSV(collision);
	newMap->setBorder(playerArea);
	tileMaps.push_back(newMap);
}

// Get Area function - returns the playerArea shape, used for fixed camera
sf::RectangleShape Room::getArea() {
	return playerArea;
}

std::vector<Lantern*> Room::getLanterns() {
	return lanterns;
}

std::vector<Chest*> Room::getChestList() {
	return chests;
}

// Draw function - Calls the draw function for each of the room's objects
void Room::draw(sf::RenderWindow* w, sf::View v) {
	// Loops through each tile map in the tileMaps vector
	for (auto& i : tileMaps) {
		i->draw(w, v);
	}

	for (auto& chest : chests) {
		w->draw(*chest);
	}
}

void Room::drawLanterns(sf::RenderWindow* window) {
	for (auto& lantern : lanterns) {
		lantern->drawLantern(window);
	}
}

// Update function - 
// Calls update function of all objects in room
// Prevents the player from moving outside the dimensions of playerArea
void Room::update(Player* p,sf::View v, float dt)  {

	// Update tile maps
	for (auto& i : tileMaps) {
		i->update(p, v, dt);
	}

	for (auto& chest : chests) {
		chest->update(p);
	}

	lanterns.front()->setPosition(p->getPosition().x + p->getSize().x / 2, p->getPosition().y + p->getSize().y / 2);

	for (int i = 1; i < lanterns.size(); i++) {
		lanterns[i]->update(p, dt);
	}

	if (p->isHoldingLantern()) {
		lanterns.front()->setFillColor(sf::Color(lanterns.front()->getFillColor().r, lanterns.front()->getFillColor().g, lanterns.front()->getFillColor().b, 255));
	} else {
		lanterns.front()->reset();
	}

	// Update doors
	for (int i = 0; i < allDoors.size(); i++) {
		allDoors[i].update(p);
	}

	// Collision checks
	if (p->getCollisionBox().left < playerArea.getPosition().x) {
		// Stop player at left side of area
		p->setPosition(playerArea.getPosition().x - (p->getSize().x - p->getCollisionBox().width)/2, p->getPosition().y);
		if (p->getDirection().x > 0.01) {
			p->setDirection(0.01, p->getDirection().y);
		}
	} else if (p->getCollisionBox().left + p->getCollisionBox().width > playerArea.getPosition().x + playerArea.getSize().x) {
		// Stop player at right side of area
		p->setPosition(playerArea.getPosition().x + playerArea.getSize().x - p->getSize().x + (p->getSize().x - p->getCollisionBox().width) / 2, p->getPosition().y);
		if (p->getDirection().x < -0.01) {
			p->setDirection(-0.01, p->getDirection().y);
		}
	}

	if (p->getCollisionBox().top < playerArea.getPosition().y) {
		// Stop player at top of area
		p->setPosition(p->getPosition().x, playerArea.getPosition().y - (p->getSize().y - p->getCollisionBox().height));
		if (p->getDirection().y > 0.01) {
			p->setDirection(p->getDirection().x, 0.01);
		}
	} else if (p->getCollisionBox().top + p->getCollisionBox().height > playerArea.getPosition().y + playerArea.getSize().y) {
		// Stop player at bottom of area
		p->setPosition(p->getPosition().x, playerArea.getPosition().y + playerArea.getSize().y - p->getSize().y);
		if (p->getDirection().y < -0.01) {
			p->setDirection(p->getDirection().x, -0.01);
		}
	}
}

// Door inputs function - calls input functions of all doors
void Room::handleInput(Input* input, int* currentRoom, Player* player, Inventory* inv) {
	for (int i = 0; i < allDoors.size(); i++) {
		allDoors[i].handleInput(input, currentRoom, player);
	}

	for (int i = 0; i < chests.size(); i++) {
		chests[i]->handleInput(input, player, inv);
	}

	for (int i = 1; i < lanterns.size(); i++) {
		lanterns[i]->handleInput(player, input);
		if (lanterns[i]->getUsed()) {
			lanterns.erase(lanterns.begin() + i);
		}
	}
}

// Load texture function - loads a texture for the room's traversable area
// For testing map designs only 
void Room::loadTexture(sf::Texture texture) {
	roomTexture = texture;
	setTexture(&roomTexture);
}