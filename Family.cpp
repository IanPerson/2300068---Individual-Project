#include "Family.h"

// Constructor
// float x -> x position of object
// float y -> y position of object
Family::Family(float x, float y) {

	// Set object properties
	setSize(sf::Vector2f(80, 80));
	setPosition(x, y);

	// Set collision box properties
	collisionBox.width = getSize().x + 20;
	collisionBox.height = getSize().y + 20;
	setCollisionBox(collisionBox);

	// Set count properties
	font.loadFromFile("font/arial.ttf");
	foodCounter.setFont(font);
	foodCounter.setFillColor(sf::Color::Black);
	foodCounter.setCharacterSize(25);
	foodCounter.setPosition(getPosition().x, getPosition().y);

	// Set initial countdown and hunger values
	food = 0;
	foodCountdown = 90;

	// Load sprite
	familyIdle.loadFromFile("gfx/Wife.png");

	// Load animation frames & speed
	for (int i = 0; i < 8; i++) {
		idle.addFrame(sf::IntRect(i * 40, 0, 40, 40));
	}
	idle.setFrameSpeed(1.f / 5.f);

	// Set animation & texture
	setTextureRect(idle.getCurrentFrame());
	setTexture(&familyIdle);
}

// Destructor
Family::~Family() {

}

// Update function
// Decreases food counter over time, and ends the game if the counter reaches 0
// If the player is colliding with the object, allows the player to give food and increase the food counter
// Updates animations and text for displaying the current food value
void Family::update(Player* p, float dt, Inventory* inventory, int* menu, bool* isPaused) {

	// Decrease countdown constantly until it reaches 0
	if (foodCountdown > 0) {
		foodCountdown -= dt;

	}
	else if (foodCountdown <= 0) {
		// If the countdown reaches 0 & the object's food value is 0, end the game
		// Otherwise, decrease the food count by one and reset the countdown to 30
		if (food == 0) {
			*menu = 6;
			*isPaused = true;
		}
		else {
			food -= 1;
			foodCountdown = 30;
			// std::cout << food << "\n";
		}
	}

	// If the player collides with the object, enable keyboard interaction and check for player input
	// If the player presses space and has a food item in the inventory, increases the food counter up to a max of 5 and removes one instance of the item from the inventory
	if (p->getCollisionBox().intersects(getCollisionBox())) {
		// Highlight object light red to show detection
		setFillColor(sf::Color(255, 205, 205, 255));

		if (food < 5) {
			p->setPrompt("F to feed");
		}

		if (interact == 1) {
			p->setInteract(p->getInteract() + 1);
			interact -= 1;
		}

		keyboardInteract = true;
		if (keyboardInteract == true && input->isPressed(sf::Keyboard::F)) {
			if (inventory->getItemList()[inventory->getIndex()].getItemID() > 3 && inventory->getItemList()[inventory->getIndex()].getItemID() < 9 && inventory->getItemList()[inventory->getIndex()].getItemQuantity() > 0 && food < 5) {
				inventory->removeFromSlot(inventory->getIndex(), false);
				food += 1;
			}
		}
	}
	else {
		// Remove highlight and disable keyboard interaction
		setFillColor(sf::Color::White);
		keyboardInteract = false;

		if (interact == 0) {
			p->setPrompt("");
			p->setInteract(p->getInteract() - 1);
			interact += 1;
		}
	}

	// Update foodCounter string
	foodCounter.setString(std::to_string((int)(food)));

	// Update fill color of the counter text based on the hunger value
	if (food <= 3) {
		foodCounter.setFillColor(sf::Color::Red);
	}
	else {
		foodCounter.setFillColor(sf::Color::Black);
	}

	// Animate object
	setTexture(&familyIdle);
	idle.animate(dt);
	setTextureRect(idle.getCurrentFrame());
}

// getText function
// Returns the foodCount text variable
sf::Text Family::getText() {
	return foodCounter;
}