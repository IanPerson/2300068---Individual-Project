#include "Item.h"
#include <iostream>

// Constructor which sets item ID
Item::Item(int id) {
	// Set ID
	itemID = id;

	// Set quantity to 1
	itemQuantity = 1;

	// Set size
	setSize({ sf::VideoMode::getDesktopMode().width / 50.f, sf::VideoMode::getDesktopMode().width / 50.f });
};

// Overloaded constructor which sets item ID & quantity
Item::Item(int id, int _itemQuantity) {
	// Set ID
	itemID = id;

	// Set quantity
	itemQuantity = _itemQuantity;
	
	// Set size
	setSize({ sf::VideoMode::getDesktopMode().width / 50.f, sf::VideoMode::getDesktopMode().width / 50.f });
};

// Deconstructor
Item::~Item() {};

// loadTexture function
// If item's ID is valid, load texture from specified location
void Item::loadTexture(std::string fileLocation) {
	if (itemID >= 0) {
		itemTexture.loadFromFile(fileLocation);
		setTexture(&itemTexture);
	}
};

// getItemID function
// Returns item's ID
int Item::getItemID() {
	return itemID;
}

// getItemQuantity function
// Returns the quantity of an item this object carries
int Item::getItemQuantity() {
	return itemQuantity;
};

// setItemID function
// Sets the item's ID to the specified value
void Item::setItemID(int id) {
	itemID = id;
};

// setItemQuantity function
// Sets the quantity of the item held to the specified amount
void Item::setItemQuantity(int _itemQuantity) {
	itemQuantity = _itemQuantity;
};

// resetItem function
// Resets item to hold empty values
void Item::resetItem() {
	itemQuantity = 0;
	itemID = -1;
};