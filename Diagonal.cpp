#include "Diagonal.h"
#include "Framework/VectorHelper.h"

// Constructor
Diagonal::Diagonal(sf::Vector2f position, sf::Vector2f p1, sf::Vector2f p2) {
	// Tile Size
	setSize({100,100});

	// Points to create collision between
	point1 = p1;
	point2 = p2;

	// Start at point 1, calculate direction to move in
	setPosition(point1);
	direction = p2 - p1;
	direction = VectorHelper::normalise(direction);
	directionNormalise.x = 1 / abs(direction.x);
	directionNormalise.y = 1 / abs(direction.y);

	// Until the object reaches point 2, create a collision box, add it to the list, and move in a certain direction
	// Currently only works for 45 degree angles
	while(getPosition() != point2){
		sf::FloatRect c;

		c.top = getPosition().y;
		c.left = getPosition().x - 10;
		c.width = 10;
		c.height = 10;

		collisionLine.push_back(c);

		setPosition(getPosition().x + round(direction.x * directionNormalise.x) * 4, getPosition().y + round(direction.y * directionNormalise.y) * 4);
	}

	// Reset object position to position parameter (for sprite rendering)
	setPosition(position);
	setFillColor(sf::Color::Red);
}

// Deconstructor
Diagonal::~Diagonal() {

}

// Depending on the position of point 1 relative to point 2, push the player back in a certain direction when colliding with the line of collision boxes
void Diagonal::update(Player* p, sf::View v, float dt) {

	if ((getPosition().x > v.getCenter().x - screenW / 8 && getPosition().x < v.getCenter().x + screenW / 8) &&
		(getPosition().y > v.getCenter().y - screenH / 8 && getPosition().y < v.getCenter().y + screenH / 8)) {
		// If point 1 is southeast of point 2, push player up and right
		if (point1.x > point2.x && point1.y > point2.y) {

			for (int i = 0; i < collisionLine.size(); i++) {
				if (p->getCollisionBox().intersects(collisionLine[i])) {
					p->setPosition(p->getPosition().x + 1.4, p->getPosition().y - 1.4);
				}
			}

		// If point 1 is northeast of point 2, push player down and right
		}
		else if (point1.x > point2.x && point1.y < point2.y) {

			for (int i = 0; i < collisionLine.size(); i++) {
				if (p->getCollisionBox().intersects(collisionLine[i])) {
					p->setPosition(p->getPosition().x + 1.4, p->getPosition().y + 1.4);
				}
			}

		// If point 1 is southwest of point 2, push player up and left
		}
		else if (point1.x < point2.x && point1.y > point2.y) {

			for (int i = 0; i < collisionLine.size(); i++) {
				if (p->getCollisionBox().intersects(collisionLine[i])) {
					p->setPosition(p->getPosition().x - 1.4, p->getPosition().y - 1.4);
				}
			}

		// If point 1 is northwest of point 2, push player down and left
		}
		else if (point1.x < point2.x && point1.y < point2.y) {

			for (int i = 0; i < collisionLine.size(); i++) {
				if (p->getCollisionBox().intersects(collisionLine[i])) {
					p->setPosition(p->getPosition().x - 1.4, p->getPosition().y + 1.4);
				}
			}

		}
	}
}