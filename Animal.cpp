#include "Animal.h"

//Constructor.
Animal::Animal(sf::Vector2f boundsPos, sf::Vector2f boundsSize) {
	// Set animal properties
	setSize(sf::Vector2f(50, 50));
	setFillColor(sf::Color::White);
	setPosition(0,0);

	// Set collision box properties
	collisionBox.width = getSize().x + 50;
	collisionBox.height = getSize().y + 50;
	collisionBox.left = -25;
	collisionBox.top = -25;
	setCollisionBox(collisionBox);

	boundaryPos = boundsPos;
	boundarySize = boundsSize;

	// Movement variables
	speed = 3;
	position.x = 0;
	position.y = 0;

	// Set initial harvest cooldown
	harvestCooldown = 0;

	// Load texture
	animalTextures.push_back(new sf::Texture);
	animalTextures[0]->loadFromFile("gfx/Chicken.png");

	// Add animation frames
	for (int i = 0; i < 4; i++) {
		idle.addFrame(sf::IntRect(i * 32, 32, 32, 32));
		walk.addFrame(sf::IntRect(i * 32, 0, 32, 32));
	}

	// Set animation speed
	idle.setFrameSpeed(1.f / 5.f);
	walk.setFrameSpeed(1.f / 5.f);

	// Set textures and animation
	setTexture(animalTextures[0]);
	currentAnimation = &idle;
	setTextureRect(currentAnimation->getCurrentFrame());
}

// Deconstructor
Animal::~Animal() {

}

// Random movement generator
int Animal::randomMovement() {
	int i = rand() % 10;
	return i;
}

void Animal::countdown(float dt) {
	// Decrease harvest cooldown until it reaches 0
	if (harvestCooldown > 0) {
		harvestCooldown -= ((rand() % 5) * dt);
	} else {
		harvestCooldown = 0;
	}
}

// Movement function
// Moves object depending on randomMovement
void Animal::movement(float dt) {
	// Get value from randomMovement for switch statement
	switch (randomMovement()) {
		// Move object up
	case 0:
		velocity += sf::Vector2f(0, -speed * dt);
		break;
		// Move object right
	case 1:
		velocity += sf::Vector2f(speed * dt, 0);
		break;
		// Move object down
	case 2:
		velocity += sf::Vector2f(0, speed * dt);
		break;
		// Move object left
	case 3:
		velocity += sf::Vector2f(-speed * dt, 0);
		break;
		// Decelerate depending on current velocity
	case 5:
		if (velocity.y < 0) {
			velocity.y += (5 * -velocity.y * dt);
		}

		if (velocity.y > 0) {
			velocity.y -= (5 * velocity.y * dt);
		}

		if (velocity.y < 0.01 && velocity.y > -0.01) {
			velocity.y = 0;
		}

		if (velocity.x < 0) {
			velocity.x += (5 * -velocity.x * dt);
		}

		if (velocity.x > 0) {
			velocity.x -= (5 * velocity.x * dt);
		}

		if (velocity.x < 0.01 && velocity.x > -0.01) {
			velocity.x = 0;
		}
		break;
		// Default to doing nothing
	default:
		break;
	}

	// Move according to velocity
	move(velocity);
}

// Update function
// Move object and check collisions
void Animal::update(Player* p, float dt) {
	// Call movement function
	movement(dt);

	// Prevent object from moving out of defined boundaries

	// Check x boundaries
	if (getPosition().x < boundaryPos.x)
	{
		setPosition(boundaryPos.x, getPosition().y);
	}
	else if (getPosition().x > boundaryPos.x + boundarySize.x)
	{
		setPosition(boundaryPos.x + boundarySize.x, getPosition().y);
	}

	// Check y boundaries
	if (getPosition().y < boundaryPos.y)
	{
		setPosition(getPosition().x, boundaryPos.y);
	}
	else if (getPosition().y > boundaryPos.y + boundarySize.y)
	{
		setPosition(getPosition().x, boundaryPos.y + boundarySize.y);
	}

	// If player collides with collision box, enable keyboard interaction
	// Otherwise disable keyboard interaction
	if (p->getCollisionBox().intersects(getCollisionBox())) {
		setFillColor(sf::Color::Yellow);
		keyboardInteract = true;

		if (harvestCooldown <= 0) {
			p->setPrompt("F to collect");
		}

		if (interact == 1) {
			p->setInteract(p->getInteract() + 1);
			interact -= 1;
		}
	} else {
		setFillColor(sf::Color::White);
		keyboardInteract = false;
		if (interact == 0) {
			p->setPrompt("");
			p->setInteract(p->getInteract() - 1);
			interact += 1;
		}
	}


	// If the object is moving, use walking animation
	// Otherwise use idle animation
	if (velocity.x < 0 && velocity.y)
	{
		currentAnimation = &walk;
		// Unflip texture if moving left
		currentAnimation->setFlipped(false);
	}
	else if (velocity.x > 0)
	{
		currentAnimation = &walk;
		// Flip texture if moving right
		currentAnimation->setFlipped(true);
	}
	else if (velocity.y != 0) {
		currentAnimation = &walk;
	}
	else {
		currentAnimation = &idle;
	}

	// Animate object
	currentAnimation->animate(dt);
	setTextureRect(currentAnimation->getCurrentFrame());
}

// Handle Input function
// Detect player input
void Animal::handleInput(Input* i, Player* p, Inventory* inv) {
	// If player interacts with object and harvest cooldown has reached 0, add item to inventory and reset cooldown
	if (keyboardInteract && i->isPressed(sf::Keyboard::F) && harvestCooldown <= 0) {
		inv->addToInventory(8, 1);
		harvestCooldown = 30;
	}
}