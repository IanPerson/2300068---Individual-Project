#include "Wall.h"
#include "Framework/Collision.h"

// Default Constructor
Wall::Wall() {
	// Set properties
	position = {100,100};
	collisionP = {0,0};
	collisionSize = { 100,100 };
	setSize(collisionSize);
	setPosition(position.x + collisionP.x, position.y + collisionP.y);

	// Set collider to true
	setCollider(true);

	// Set collision properties
	collisionBox.left = 0;
	collisionBox.top = 0;
	collisionBox.width = collisionSize.x;
	collisionBox.height = collisionSize.y;

	// Set collision box if collision size is valid
	if (collisionSize.x != 0 && collisionSize.y != 0) {
		setCollisionBox(collisionBox);
	}
};

// Constructor
Wall::Wall(sf::Vector2f p, sf::Vector2f boxPos, sf::Vector2f boxSize) {
	// Set properties
	position = p;
	collisionP = boxPos;
	collisionSize = boxSize;
	setSize(boxSize);
	setPosition(position.x + collisionP.x, position.y + collisionP.y);

	// Set collider to true
	setCollider(true);

	// Set collision properties
	collisionBox.left = 0;
	collisionBox.top = 0;
	collisionBox.width = collisionSize.x;
	collisionBox.height = collisionSize.y;

	// Set collision box if collision size is valid
	if (boxSize.x != 0 && boxSize.y != 0) {
		setCollisionBox(collisionBox);
	}

};

// Deconstructor
Wall::~Wall() {};


// Update function
// Only check collision if within a certain distance of player
// If the player collides with wall, check the direction of impact and push opposite direction
void Wall::update(Player* p, sf::View v, float dt) {

	// Check distance according to camera (distance adjusts to screen resolution)
	if ((getPosition().x > v.getCenter().x - screenX / 3 && getPosition().x < v.getCenter().x + screenX / 3) &&
		(getPosition().y > v.getCenter().y - screenY && getPosition().y < v.getCenter().y + screenY)) {
		// If the player's collision box overlaps with the wall's collision box, increase player's current collisions by 1 and call collisionResponse
		if (p->getCollisionBox().intersects(getCollisionBox()) && p->getCurrentCollisions() <= 1) {
			p->setCurrentCollisions(p->getCurrentCollisions()+1);
			p->collisionResponse(this);
		}
		
		// If player is not colliding with object and player's current collisions is greater than 0, decrease current collisions by 1
		if (!getCollisionBox().intersects(getCollisionBox()) && p->getCurrentCollisions() > 0) {
			p->setCurrentCollisions(p->getCurrentCollisions() - 1);
		}
	}
};

void Wall::update(Chaser* p, sf::View v, float dt) {

	// Check distance according to camera (distance adjusts to screen resolution)
	if ((getPosition().x > v.getCenter().x - screenX / 3 && getPosition().x < v.getCenter().x + screenX / 3) &&
		(getPosition().y > v.getCenter().y - screenY && getPosition().y < v.getCenter().y + screenY)) {
		// If the player's collision box overlaps with the wall's collision box, increase player's current collisions by 1 and call collisionResponse
		if (p->getCollisionBox().intersects(getCollisionBox())) {
			p->collisionResponse(this);
		}
	}
};

