#include "Chaser.h"
#include "Framework/VectorHelper.h"
#include "Framework/Collision.h"

Chaser::Chaser(sf::Vector2f pos) {
	// Load texture
	texture.loadFromFile("gfx/Chaser.png");
	setTexture(&texture);

	// Set position & size
	setSize({ 100,200 });
	setPosition(pos);

	// Sprite and animation
	idle.addFrame(sf::IntRect(0,0,40,80));
	setTextureRect(idle.getCurrentFrame());
	setFillColor(sf::Color::White);

	// Collision boxes
	innerDetectionCircle.setSize({ getSize().x + 400, getSize().y + 400});
	outerDetectionCircle.setSize({ getSize().x + 600, getSize().y + 600 });
	hitBox.setSize({ getSize().x+100, getSize().y+100 });

	// Movement
	speed = { 350, 350 };
	dashSpeed = { 0, 0 };

	// Default to not hostile
	// Set cooldowns to 0
	hasAggro = false;
	attackCooldown = 0;
	attackDuration = 0;
	moveCooldown = 0;
	moveDuration = 0;
	isAttacking = false;

	// Hitbox color - testing only
	hitBox.setFillColor(sf::Color::Blue);
}

// Update function
// Enemy will move around randomly with no player to chase
// Once the player is seen, enemy will follow and attack
void Chaser::update(Player* p, float dt, std::vector<Lantern*> l) {
	// Detection
	detectPlayer(p, dt);

	// Flip sprite to face left or right
	if (speed.x > 0) {
		idle.setFlipped(false);
	} else {
		idle.setFlipped(true);
	}

	// Run away if colliding with a lantern
	for (auto& lantern : l) {
		if (Collision::checkBoundingCircle(this, lantern)) {
			fleeing = true;
			fleeDuration = 5;
			direction = -direction;
		}
	}

	// When fleeing, run in a certain direction for some time before returning to non-hostile state
	if (fleeing) {
		if (fleeDuration > 0) {
			fleeDuration -= dt;
			move(direction.x * speed.x / 2 * dt, direction.y * speed.y / 2 * dt);
		} else {
			fleeing = false;
		}
	// When chasing the player, accelerate to max speed and attack when close enough
	} else if (hasAggro) {
		setFillColor(sf::Color::Red);

		// Determine direction to move towards player
		direction.x = distanceFrom.x;
		direction.y = distanceFrom.y;
		direction = VectorHelper::normalise(direction);

		// Accelerate
		if (dashSpeed.x < 550 && dashSpeed.y < 550 && !isAttacking) {
			dashSpeed.x += 1100 * dt;
			dashSpeed.y += 1100 * dt;
		}

		// When colliding with player, enable interaction & trigger attack
		if (Collision::checkBoundingCircle(p, this)) {
			keyboardInteract = true;
			isAttacking = true;
		// Move towards player otherwise
		} else {
			move(direction.x * dashSpeed.x * dt, direction.y * dashSpeed.y * dt);
			keyboardInteract = false;
		}
	// Reset dash speed and move randomly otherwise
	} else {
		dashSpeed = { 0,0 };
		setFillColor(sf::Color::White);
		// Move in a random direction periodically
		if (moveCooldown < 2) {
			moveCooldown += dt;
			direction.x = rand() % 101 / 50.f - 1;
			direction.y = rand() % 101 / 50.f - 1;
		} else {
			if (moveDuration < 0.5) {
				moveDuration += dt;
				move(direction.x * speed.x/2 * dt, direction.y * speed.y/2 * dt);
			} else {
				moveCooldown = 0;
				moveDuration = 0;
			}
		}
	}

	// Stop and carry out attack when triggered
	if (isAttacking) {
		attack(p, dt);
		dashSpeed = { 0,0 };
	}

	// Move hitbox
	hitBox.setPosition(getPosition().x - 50, getPosition().y - 50);
}

// handleInput function
// If player is in range and presses R, they will use their held lantern to kill this enemy
void Chaser::handleInput(Player* p, Input* input, bool* k) {
	if (input->isPressed(sf::Keyboard::R) && keyboardInteract && p->isHoldingLantern()) {
		p->carryLantern(false);
		*k = true;
	}
}

// Attack function
// For a period of time, enables a hitbox which will hurt the player if in range
// Reset to not attacking afterwards
void Chaser::attack(Player* p, float dt) {

	if (attackCooldown < 0.1) {
		setFillColor(sf::Color::Black);
		attackCooldown += dt;
		direction = { 0,0 };
	} else if (attackDuration < 0.2) {
		attackDuration += dt;
		if (Collision::checkBoundingCircle(p, &hitBox)) {
			hitBox.setFillColor(sf::Color::Yellow);
			if (!playerHit) {
				p->takeDamage(1);
				playerHit = true;
			}
		}
	} else {
		attackCooldown = 0;
		attackDuration = 0;
		playerHit = false;
		hitBox.setFillColor(sf::Color::Blue);
		isAttacking = false;
	}
}

// Draw enemy to window
void Chaser::draw(sf::RenderWindow* w) {
	if (attackCooldown >= 0.1) {
		w->draw(hitBox);
	}
	w->draw(*this);
}

// collisionResponse function
// Handle collision with other solid objects, identical to player
void Chaser::collisionResponse(GameObject* o) {
	if (currentCollisions <= 2) {

		// Get position of enemy's center relative to object's center
		float diffx = abs((getCollisionBox().left + getCollisionBox().width / 2) - (o->getCollisionBox().left + o->getCollisionBox().width / 2));
		float diffy = abs((getCollisionBox().top + getCollisionBox().height / 2) - (o->getCollisionBox().top + o->getCollisionBox().height / 2));

		// If the enemy's relative x position is less than the enemy's relative y position, do y collision checks
		// Otherwise do x collision checks
		if (diffx < diffy - 10) {
			// If enemy is above object, push enemy up
			// Otherwise push enemy down
			if (getCollisionBox().top + getCollisionBox().height / 2 < o->getCollisionBox().top + o->getCollisionBox().height / 2) {
				// Set position to top of object
				setPosition(getPosition().x, o->getCollisionBox().top - 80);
				// Limit y direction
				if (direction.y > 0.01) {
					direction.y = 0.01;
				}
			} else {
				// Set position to bottom of object
				setPosition(getPosition().x, o->getCollisionBox().top + o->getCollisionBox().height - 40);
				// Limit y direction
				if (direction.y < -0.01) {
					direction.y = -0.01;
				}
			}
		} else {
			// If enemy is to the left of object, push enemy left
			// Otherwise push enemy right
			if (getCollisionBox().left + getCollisionBox().width / 2 < o->getCollisionBox().left + o->getCollisionBox().width / 2) {
				// Set position to left side of object
				setPosition(o->getCollisionBox().left - 50, getPosition().y);
				// Limit x direction
				if (direction.x > 0.01) {
					direction.x = 0.01;
				}
			} else {
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