#include "House.h"

House::House(sf::Vector2f pos, int doorNum) {
	setPosition(pos);
	setSize({500,500});

	collisionBox.width = getSize().x;
	collisionBox.height = getSize().y;

	setCollisionBox(collisionBox);

	texture.loadFromFile("gfx/House.png");
	setTexture(&texture);

	inside = new Room({1100,1000});
	inside->addTileMap(0, "Maps/Map2_Grass.csv", "Maps/Map2_Collision.csv");
	inside->addTileMap(5, "Maps/Map2_House.csv");

	entrance.setPosition({getPosition().x + getSize().x/2 - entrance.getSize().x/2, getPosition().y + getSize().y - entrance.getSize().y});
	entrance.setMoveTo(doorNum);

	inside->addDoor({ inside->getSize().x / 2 - 50, inside->getSize().y - 1000 }, { entrance.getPosition().x, entrance.getPosition().y + 75}, 0, 0);
	entrance.setMoveCoords({ inside->getSize().x / 2 - 50, inside->getSize().y - 1000 });
}

void House::update(Player* p) {
	if (p->getCollisionBox().intersects(getCollisionBox())) {
		p->collisionResponse(this);
	}

	entrance.update(p);
}

void House::handleInput(Input* i, int* currentRoom, Player* p) {
	entrance.handleInput(i, currentRoom, p);
}

Room* House::getRoom() {
	return inside;
}