#include "Grabber.h"
#include "Framework/Collision.h"
#include "Framework/VectorHelper.h"

Grabber::Grabber(sf::Vector2f pos) {
	// Textures
	texture.loadFromFile("gfx/Nest.png");
	limbTexture.loadFromFile("gfx/Nest_Arm.png");
	handTexture.loadFromFile("gfx/Nest_Hand.png");

	setTexture(&texture);
	setFillColor(sf::Color::White);

	// Detection and range
	outerDetectionCircle.setSize({1150,1150});
	innerDetectionCircle.setSize({600, 600});
	maxRange.setSize({ 800,800 });

	// Hand, arm, and body properties
	grabBox.setSize({50,50});
	arm.setSize({25,30});
	arm.setTexture(&limbTexture);
	setSize({ 100,100 });
	setPosition(pos);

	// Set speed for retracting arm
	retractSpeed = 800;

	// Hand and arm positions
	grabBox.setPosition(pos.x + getSize().x / 2, pos.y + getSize().y / 2);
	grabBox.setOrigin(grabBox.getSize().x / 2, grabBox.getSize().y / 2);
	grabBox.setTexture(&handTexture);
	arm.setOrigin({ arm.getSize().x, arm.getSize().y/2});
	arm.setPosition(pos.x + getSize().x / 2, pos.y + getSize().y / 2);

	// Line to track player locationn
	line[0].position.x = pos.x + getSize().x / 2;
	line[0].position.y = pos.y + getSize().y / 2;
	line[1].position.x = pos.x + getSize().x / 2;
	line[1].position.y = pos.y + getSize().y / 2;

	inBounds = true;
}

// Update function
// Enemy will attempt to grab player when within range
// Player will start taking damage after being pulled in fully
void Grabber::update(Player* p, float dt) {
	// Detect player
	detectPlayer(p, dt);

	// Set max range and check if tracking line is in bounds
	maxRange.setPosition({ getPosition().x + getSize().x / 2 - maxRange.getSize().x / 2, getPosition().y + getSize().y / 2 - maxRange.getSize().y / 2 });
	inBounds = pow(maxRange.getPosition().x + maxRange.getSize().x / 2 - line[1].position.x, 2) + pow(maxRange.getPosition().y + maxRange.getSize().y / 2 - line[1].position.y, 2) < pow(maxRange.getSize().x / 2, 2);

	// Record distance between the hand and body
	armDistance.x = grabBox.getPosition().x - getPosition().x - getSize().x / 2;
	armDistance.y = grabBox.getPosition().y - getPosition().y - getSize().y / 2;

	// Set arm position to center of body
	arm.setPosition(getPosition().x + getSize().x / 2, getPosition().y + getSize().y / 2);
	// Increase length of arm depending on how far extended the hand is
	arm.setSize({ (float)sqrt(pow(armDistance.x, 2) + pow(armDistance.y, 2)) + 20, 30});

	// Get angle for arm rotation
	armAngle = atan2(armDistance.y, armDistance.x) * 180 / 3.141f;

	// Check if player is invulnerable to being pulled before attempting to grab
	if (hasAggro && p->getInvulnerability() == 0) {
		innerDetectionCircle.setFillColor(sf::Color::Red);

		// Accelerate hand
		if (speed.x < 700 && speed.y < 700) {
			speed.x += 450 * dt;
			speed.y += 450 * dt;
		}

		// Get direction to move hand towards player
		direction.x = p->getPosition().x + p->getSize().x/2 - line[1].position.x;
		direction.y = p->getPosition().y + p->getSize().y/2 - line[1].position.y;

		// Check distance between the tracking point and the body
		lineDistance.x = line[1].position.x - getPosition().x - getSize().x / 2;
		lineDistance.y = line[1].position.y - getPosition().y - getSize().y / 2;

		// Check distance between hand and player
		grabDistance.x = grabBox.getPosition().x - p->getPosition().x - p->getSize().x / 2;
		grabDistance.y = grabBox.getPosition().y - p->getPosition().y - p->getSize().y / 2;

		// Normalise directions
		direction = VectorHelper::normalise(direction);
		lineDistance = VectorHelper::normalise(lineDistance);
		grabDistance = VectorHelper::normalise(grabDistance);
		armDistance = VectorHelper::normalise(armDistance);

		// Get angles
		lineAngle = atan2(lineDistance.x, lineDistance.y);
		grabAngle = atan2(grabDistance.y, grabDistance.x) * 180 / 3.141f;

		// Get edge coordinates from max range
		circleEdge.x = maxRange.getPosition().x + maxRange.getSize().x/2 + maxRange.getSize().x/2 * sin(lineAngle) + lineDistance.x;
		circleEdge.y = maxRange.getPosition().y + maxRange.getSize().x / 2 + maxRange.getSize().x/2 * cos(lineAngle) + lineDistance.y;
		
		// Move tracking point towards player when in range
		if (Collision::checkBoundingCircle(p, &maxRange) || inBounds) {
			line[1].position.x += direction.x * speed.x * dt;
			line[1].position.y += direction.y * speed.y * dt;
		} else {
			line[1].position.x += direction.x * speed.x * 2/3 * dt;
			line[1].position.y += direction.y * speed.y * 2/3 * dt;
		}

		// If player leaves the max range or gets grabbed, retract arm
		if (Collision::checkBoundingCircle(p, &grabBox) && Collision::checkBoundingCircle(p, &maxRange)) {
			retract(p, true, dt);
		// If hand attempts to leave max range, set hand position to edge
		} else if (!inBounds) {
			grabBox.setPosition(circleEdge.x, circleEdge.y);
		// Otherwise, set hand position to position of tracking point
		} else {
			grabBox.setPosition(line[1].position.x, line[1].position.y);
		}

		// When the body collides with the player, start attacking
		if (Collision::checkBoundingBox(p, this)) {
			attackCooldown -= dt;
			if (attackCooldown <= 0) {
				attackCooldown = 1.5;
				p->takeDamage(1);
			}
		}

		// Rotate hand to face player
		grabBox.setRotation(grabAngle);
	} else {
		// Retract arm when player is out of range
		speed = {0, 0};
		innerDetectionCircle.setFillColor(sf::Color::White);
		retract(p, false, dt);
	}

	// Rotate arm to match hand
	arm.setRotation(armAngle);
}

// handleInput function
// If player is in range and presses R, they will use their held lantern to kill this enemy
void Grabber::handleInput(Player* p, Input* input, bool* k) {
	if (input->isPressed(sf::Keyboard::R) && keyboardInteract && p->isHoldingLantern()) {
		p->carryLantern(false);
		*k = true;
	}
}

// Retract function
// Shrink arm and hand back into the main body, and trap player if they were grabbed
void Grabber::retract(Player* p, bool grabbing, float dt) {

	// Decelerating speed for retraction
	if (retractSpeed > 200) {
		retractSpeed -= 600 * dt;
	}

	// Update line distance
	lineDistance.x = line[1].position.x - getPosition().x - getSize().x / 2;
	lineDistance.y = line[1].position.y - getPosition().y - getSize().y / 2;

	// Move line and hand towards body
	if (abs(lineDistance.x) > 5 || abs(lineDistance.y) > 5) {
		lineDistance = VectorHelper::normalise(lineDistance);
		line[1].position.x -= lineDistance.x * retractSpeed * dt;
		line[1].position.y -= lineDistance.y * retractSpeed * dt;
	} else {
		line[1].position.x = getPosition().x + getSize().x / 2;
		line[1].position.y = getPosition().y + getSize().y / 2;
		retractSpeed = 800;
		grabBox.setRotation(0);
	}

	// If holding onto the player, they must press 10 keys before being able to escape
	if (grabbing) {
		grabBox.setPosition(line[1].position.x, line[1].position.y);
		p->setPosition(line[1].position.x - p->getSize().x / 2, line[1].position.y - p->getSize().y / 2);
		p->enableKeys(false);

		for (int i = 0; i < 256; i++) {
			if (input->isPressed(i)) {
				keyPressCount++;
				if (keyPressCount > 10) {
					p->takeDamage(0);
					p->enableKeys(true);
					p->move(0, 50);
					keyPressCount = 0;
					break;
				}
			}
		}
	// Set hand position to edge of range if out of bounds
	} else if (!inBounds) {
		grabBox.setPosition(circleEdge.x, circleEdge.y);
	// Set hand position to tracking point otherwise
	} else {
		grabBox.setPosition(line[1].position.x, line[1].position.y);
		p->enableKeys(true);
	}
}

// Draw enemy to window
void Grabber::draw(sf::RenderWindow* w) {
	w->draw(arm);
	w->draw(grabBox);
	w->draw(*this);
}

// collisionResponse function
// Handle collision with other solid objects, identical to player
void Grabber::collisionResponse(GameObject* o) {
	if (currentCollisions <= 2) {

		// Get position of player's center relative to object's center
		float diffx = abs((getCollisionBox().left + getCollisionBox().width / 2) - (o->getCollisionBox().left + o->getCollisionBox().width / 2));
		float diffy = abs((getCollisionBox().top + getCollisionBox().height / 2) - (o->getCollisionBox().top + o->getCollisionBox().height / 2));

		// If the player's relative x position is less than the player's relative y position, do y collision checks
		// Otherwise do x collision checks
		if (diffx < diffy - 10) {
			// If player is above object, push player up
			// Otherwise push player down
			if (getCollisionBox().top + getCollisionBox().height / 2 < o->getCollisionBox().top + o->getCollisionBox().height / 2) {
				// Set position to top of object
				setPosition(getPosition().x, o->getCollisionBox().top - 80);
				// Limit y direction
				if (direction.y > 0.01) {
					direction.y = 0.01;
				}
			}
			else {
				// Set position to bottom of object
				setPosition(getPosition().x, o->getCollisionBox().top + o->getCollisionBox().height - 40);
				// Limit y direction
				if (direction.y < -0.01) {
					direction.y = -0.01;
				}
			}
		}
		else {
			// If player is to the left of object, push player left
			// Otherwise push player right
			if (getCollisionBox().left + getCollisionBox().width / 2 < o->getCollisionBox().left + o->getCollisionBox().width / 2) {
				// Set position to left side of object
				setPosition(o->getCollisionBox().left - 50, getPosition().y);
				// Limit x direction
				if (direction.x > 0.01) {
					direction.x = 0.01;
				}
			}
			else {
				// Set position to right side of object
				setPosition(o->getCollisionBox().left + o->getCollisionBox().width - 30, getPosition().y);
				// Limit x direction
				if (direction.x < -0.01) {
					direction.x = -0.01;
				}
			}
		}
	}
}

void Grabber::setTextures(sf::Texture* a, sf::Texture* h) {
	arm.setTexture(a);
	grabBox.setTexture(h);
}