#include "Player.h"
#include <iostream>

// Constructor
Player::Player() {

	// Default current collisions to 0
	currentCollisions = 0;

	keysEnabled = true;
	health = 3;
	invulnerability = 0;

	promptFont.loadFromFile("font/arial.ttf");
	prompt.setFont(promptFont);
	prompt.setOutlineColor(sf::Color::Black);
	prompt.setFillColor(sf::Color::White);
	prompt.setCharacterSize(25);

	// Set object properties
	setSize({ 80, 80 });
	setPosition({ 2000,2000 });

	// Set collision box properties
	collisionBox.width = getSize().x / 4;
	collisionBox.height = getSize().y / 2;
	collisionBox.left = 30;
	collisionBox.top = 40;
	setCollisionBox(collisionBox);

	// Set hitbox size to match collision box
	hitbox.setSize({ collisionBox.width, collisionBox.height });

	// Movement variables
	speed.x = 0;
	speed.y = 0;

	// Set input
	setInput(input);

	// Initialize idle animation
	playerIdle.loadFromFile("gfx/PlayerIdle.png");

	for (int i = 0; i < 8; i++) {
		idle.addFrame(sf::IntRect(i * 40, 0, 40, 40));
	}
	idle.setFrameSpeed(1.f / 5.f);

	// Initialize walk animation
	playerWalk.loadFromFile("gfx/PlayerRun.png");

	walk.addFrame(sf::IntRect(0, 0, 40, 40));
	walk.addFrame(sf::IntRect(80, 0, 40, 40));
	walk.addFrame(sf::IntRect(120, 0, 40, 40));
	walk.addFrame(sf::IntRect(160, 0, 40, 40));
	walk.addFrame(sf::IntRect(240, 0, 40, 40));
	walk.addFrame(sf::IntRect(280, 0, 40, 40));
	walk.setFrameSpeed(1.f / 10.f);

	// Set current animation
	currentAnimation = &idle;
	setTextureRect(currentAnimation->getCurrentFrame());
}

// Deconstructor
Player::~Player() {

}

// Handle input function -
// Moves player in a direction according to arrow keys held
void Player::handleInput(float dt) {
	// Variables to store input
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	bool shift = false;

	if (keysEnabled) {
		up = input->isKeyDown(sf::Keyboard::W) && keysEnabled;
		down = input->isKeyDown(sf::Keyboard::S) && keysEnabled;
		left = input->isKeyDown(sf::Keyboard::A) && keysEnabled;
		right = input->isKeyDown(sf::Keyboard::D) && keysEnabled;
		shift = input->isKeyDown(sf::Keyboard::LShift) || input->isKeyDown(sf::Keyboard::RShift);
	}

	// Change player's x direction depending on input
	// Decelerate to 0 if both keys are pressed, or if neither are pressed
	if (left && !right) {
		if (direction.x > -1) {
			direction.x -= 5 * dt;
		}
	}
	else if (right && !left) {
		if (direction.x < 1) {
			direction.x += 5 * dt;
		}
	}
	else {
		if (direction.x > 0.1) {
			direction.x -= 5 * dt;
		}
		else if (direction.x < -0.1) {
			direction.x += 5 * dt;
		}
		else {
			direction.x = 0;
		}
	}

	// Change player's y direction depending on input
	// Decelerate to 0 if both keys are pressed, or if neither are pressed
	if (up && !down) {
		if (direction.y > -1) {
			direction.y -= 5 * dt;
		}
	}
	else if (down && !up) {
		if (direction.y < 1) {
			direction.y += 5 * dt;
		}
	}
	else {
		if (direction.y > 0.1) {
			direction.y -= 5 * dt;
		}
		else if (direction.y < -0.1) {
			direction.y += 5 * dt;
		}
		else {
			direction.y = 0;
		}
	}

	// If the player is moving both vertically and horizontally, adjust speed
	// Otherwise set speed vectors to 500
	// Prevents player from moving faster when moving diagonally
	if ((up || down) && (left || right)) {
		speed.x = sqrt(pow(health * (300 / 3) + 200, 2) / 2);
		speed.y = sqrt(pow(health * (300 / 3) + 200, 2) / 2);
	}
	else {
		speed.x = health * (300/3) + 200;
		speed.y = health * (300/3) + 200;
	}

	if (shift && sprintCooldown > 0 && !sprintDisabled) {
		speed.x *= 1.3;
		speed.y *= 1.3;
		sprintCooldown -= dt;
	} else if(shift && sprintCooldown <= 0){
		sprintDisabled = true;
		if (sprintCooldown <= 2) {
			sprintCooldown += dt / 3;
		} else {
			sprintDisabled = false;
		}
	} else {
		if (sprintCooldown <= 2) {
			sprintCooldown += dt/2;
		} else if(sprintDisabled) {
			sprintDisabled = false;
		}
	}
}

// Update function
// Move collision box to match player's location
void Player::update(float dt, bool* isPaused, int* menu) {

	if (health == 0) {
		*isPaused = true;
		*menu = 6;
	}

	if (invulnerability > 0) {
		invulnerability -= 200 * dt;
	} else {
		invulnerability = 0;
	}

	if (holdingLantern) {
		prompt.setString(std::to_string((int)lanternDuration));
		if (numInteract == 0) {
			numInteract = 1;
		}

		if (lanternDuration > 0) {
			lanternDuration -= dt;
		} else {
			holdingLantern = false;
		}


	}

	// If player's velocity is 0, use idle animation
	// Otherwise use walking animation
	if (velocity.x == 0 && velocity.y == 0)
	{
		if (getTexture() != &playerIdle) {
			walk.reset();
			setTexture(&playerIdle);
			currentAnimation = &idle;
		}
	}
	else if (velocity.x != 0 || velocity.y != 0) {
		// Have textures face left if x velocity is less than 0
		if (velocity.x < 0) {
			walk.setFlipped(true);
			idle.setFlipped(true);
		}
		// Have textures face right if x velocity is greater than 0
		else if (velocity.x > 0) {
			walk.setFlipped(false);
			idle.setFlipped(false);
		}

		// Use walking animation if not already using it
		if (getTexture() != &playerWalk) {
			setTexture(&playerWalk);
			currentAnimation = &walk;
		}
	}

	// Animate player
	currentAnimation->animate(dt);
	setTextureRect(currentAnimation->getCurrentFrame());

	// Update velocity based on speed and direction
	setVelocity(speed.x * direction.x, speed.y * direction.y);

	// Move player according to velocity
	move(velocity.x * dt, velocity.y * dt);

	prompt.setPosition({ getPosition().x + 40, getPosition().y - 20 });

	// Update position of hitbox to match collision box
	// hitbox.setPosition(getPosition().x + collisionBox.left, getPosition().y + collisionBox.top);
}

// setDirection function
// Set direction vector according to x & y parameters
void Player::setDirection(float x, float y) {
	direction.x = x;
	direction.y = y;
}

// getDirection function
// Returns direction vector
sf::Vector2f Player::getDirection() {
	return direction;
}

// getCurrentCollision function
// Return currentCollision variable
int Player::getCurrentCollisions() {
	return currentCollisions;
}

// setCurrentCollision function
// Set currentCollision variable according to parameter
void Player::setCurrentCollisions(int amount) {
	currentCollisions = amount;
}

void Player::setPrompt(std::string text) {
	if (!holdingLantern) {
		prompt.setString(text);
	}
}

void Player::setInteract(int amount) {
	numInteract = amount;
}

int Player::getInteract() {
	return numInteract;
}

// collisionResponse function
// Updates position of player based on GameObject parameter
void Player::collisionResponse(GameObject* o) {

	// Check if the player is already colliding with an object before activating collision response
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

// Draw function
// Draws player to given window
// Draws hitbox to window if enabled
void Player::draw(sf::RenderWindow* w) {
	w->draw(*this);
	//w->draw(hitbox);
	if (numInteract > 0) {
		w->draw(prompt);
	}
}

// Stop function
// Sets direction vectors to 0 and resets to idle animation
void Player::stop() {
	// Set direction vectors to 0
	direction.x = 0;
	direction.y = 0;

	// Reset idle animation and set texture to idle texture
	idle.reset();
	currentAnimation = &idle;
	setTexture(&playerIdle);
}

void Player::takeDamage(int amount) {
	if (health - amount > 0) {
		health -= amount;
	} else {
		health = 0;
	}

	invulnerability = 100;
}

void Player::enableKeys(bool value) {
	keysEnabled = value;
}

float Player::getInvulnerability() {
	return invulnerability;
}

void Player::carryLantern(bool b) {
	holdingLantern = b;
	if (b) {
		lanternDuration = 10;
	} else if(numInteract > 0) {
		numInteract -= 1;
	}
}

bool Player::isHoldingLantern() {
	return holdingLantern;
}