#include "crop.h"

// Constructor
Crop::Crop(float x, float y) {
	// Set ID to -1 (empty)
	cropID = -1;

	// Set plant & harvest states to false
	cropHarvestReady = false;
	isPlanted = false;

	// Set size & collision size
	setSize({ 100, 100 });
	collisionBox.width = getSize().x * 3/4;
	collisionBox.height = getSize().y * 2/3;

	// Set positions
	setPosition({ x, y });
	collisionBox.left = 15;
	collisionBox.top = 15;

	// Set plant sprite properties
	plantSprite.setSize({ 80, 160 });
	plantSprite.setPosition(getPosition().x + (getSize().x / 2) - (plantSprite.getSize().x / 2), (getPosition().y + (getSize().y / 2) - (plantSprite.getSize().y / 2)) - 15);

	// Load textures
	wheat.loadFromFile("gfx/Wheat.png");
	potato.loadFromFile("gfx/Potato.png");
	carrot.loadFromFile("gfx/Carrot.png");
	onion.loadFromFile("gfx/Onion.png");

	// Add frames to animations
	planted.addFrame(sf::IntRect(32, 0, 16, 32));
	growing.addFrame(sf::IntRect(48, 0, 16, 32));
	ready.addFrame(sf::IntRect(64, 0, 16, 32));

	// Set current animation
	currentAnimation = &planted;

	// Set texture rect
	plantSprite.setTextureRect(currentAnimation->getCurrentFrame());

	// Set color to white (appears green until updated otherwise)
	setFillColor(sf::Color::White);
};

// plantCrop function
// Starts growing process according to the given ID
void Crop::plantCrop(int id, Inventory* inv) {
	// Remove seed from inventory
	inv->removeFromSlot(inv->getIndex(), false);

	// Set planted to true
	isPlanted = true;

	// Reset growth
    cropGrowthLevel = 0;

	// Set ID of crop and start planted animation
	cropID = id;
	currentAnimation = &planted;

	// Set growth rate and texture according to ID
	switch (id) {
		case 0:
			cropGrowthRate = 0.0058;
			plantSprite.setTexture(&wheat);
			break;
		case 1:
			cropGrowthRate = 0.0068;
			plantSprite.setTexture(&potato);
			break;
		case 2:
			cropGrowthRate = 0.0061;
			plantSprite.setTexture(&carrot);
			break;
		case 3:
			cropGrowthRate = 0.0040;
			plantSprite.setTexture(&onion);
			break;
	}
};

// harvestCrop function
// Adds seeds & food to inventory and resets crop
void Crop::harvestCrop(Inventory* inv) {
	// Reset planted & harvest states to false
	isPlanted = false;
	cropHarvestReady = false;

	// Reset growth level
	cropGrowthLevel = 0;

	// Give random number of seeds & food
	inv->addToInventory(cropID + 4, rand() % 2 + 1);
	inv->addToInventory(cropID, rand() % 2);
};


// cropGrowth function
// Increases growth level over time and updates texture depending on growth level
void Crop::cropGrowth() {
    // Increase growth level (up to 10)
	if (cropGrowthLevel < 10) {
		cropGrowthLevel += cropGrowthRate;
	} else if (cropGrowthLevel > 10) {
		cropGrowthLevel = 10;
	}

    // Set animation according to growth level
	// If growth level reaches 10, set harvest state to true
	if (cropGrowthLevel >= 10)
	{
		currentAnimation = &ready;
		cropHarvestReady = true;
	}
	else if (cropGrowthLevel >= 5)
	{
		currentAnimation = &growing;
	}
	else
	{
		currentAnimation = &planted;
	}

};

// Update function
// Check collision with player & display growth progress
void Crop::update(Player* p, float dt) {
	// If colliding with player, highlight crop red and enable keyboard interaction
	// Otherwise, disable keyboard interaction
	if (p->getCollisionBox().intersects(getCollisionBox())) {
		setFillColor(sf::Color::Red);
		keyboardInteract = true;

		if (cropHarvestReady) {
			p->setPrompt("F to harvest");
		} else if (!isPlanted) {
			p->setPrompt("F to plant");
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

	// If something has been planted, make plant sprite visible and show progress
	// Otherwise, make plant sprite invisible
	if (isPlanted) {
		plantSprite.setFillColor(sf::Color::White);
		plantSprite.setTextureRect(currentAnimation->getCurrentFrame());
	} else {
		plantSprite.setFillColor(sf::Color::Transparent);
	}
}

// getPlant function
// Returns plant object
sf::RectangleShape Crop::getPlant()
{
	return plantSprite;
}

// handleInput function
// Plant or harvest crop when space is pressed
void Crop::handleInput(Player p, Input* i, Inventory* inv) {
	// Check if space is pressed and nothing is growing in this crop
	if (i->isKeyDown(sf::Keyboard::F) && keyboardInteract && !isPlanted) {
		// Check if held item is a type of seed
		if (inv->getItemList()[inv->getIndex()].getItemID() < 4 && inv->getItemList()[inv->getIndex()].getItemQuantity() > 0) {
			// Plant seed
			plantCrop(inv->getItemList()[inv->getIndex()].getItemID(), inv);
		}
	// Check if space is pressed and harvest state is true
	} else if (i->isKeyDown(sf::Keyboard::F) && keyboardInteract && cropHarvestReady) {
		// Harvest crop
		harvestCrop(inv);
	}
}

// getPlanted
// Returns value of isPlanted
bool Crop::getPlanted() {
	return isPlanted;
}
