#include "Inventory.h"
#include <string>

// Constructor
Inventory::Inventory(int r, int c) {
	// Default gold
	gold = 100;

	// Load font
	font.loadFromFile("font/arial.ttf");
	
	// Load item textures
	for (int i = 1; i < 10; i++) {
		sf::Texture texture;
		texture.loadFromFile("gfx/Items/" + std::to_string(i) + ".png");
		textureList.push_back(texture);
	}

	// # of rows and columns the inventory will contain
	rows = r;
	columns = c;

	// Variable defaults
	itemIndex = 0;
	holdingItem = false;
}

// Initialize function
// Initializes most values and objects within class
void Inventory::initialize() {

	// Color, Size & Position of Inventory Background
	setFillColor(sf::Color(50,150,50,255));
	setSize(sf::Vector2f(screenX * columns / 25.5, screenY * (rows-1) / 16));
	setPosition(sf::Vector2f(screenX / 100, screenY - getSize().y - (getSize().y/rows) * 1.5 ));

	// Color, Size & Position of Inventory Hotbar
	hotbar.setSize({getSize().x, getSize().y * 5 / (4.f * rows)});
	hotbar.setPosition(sf::Vector2f(getPosition().x, getPosition().y + getSize().y));
	hotbar.setFillColor(sf::Color(50, 150, 50, 255));

	// Overlay to darken screen when in inventory
	overlay.setSize(sf::Vector2f(screenX, screenY));
	overlay.setFillColor(sf::Color(0,0,0,95));

	// Reserve spaces in vector for every item
	allIcons.reserve(rows * columns);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {

			// If creating objects in the hotbar, set position according to the position of hotbar
			if (i == rows - 1) {
				// Create inventory spaces
				GameObject tile;
				tile.setFillColor(sf::Color::Green);
				tile.setSize({ screenX / 30.f, screenY / 20.f });
				tile.setPosition({ hotbar.getPosition().x + screenX * j / 26 + screenX / 200, hotbar.getPosition().y + hotbar.getSize().y/2 - tile.getSize().y/2});

				// Create blank items containing nothing
				Item item(-1, 0);
				item.setPosition({ tile.getPosition().x + tile.getSize().x / 2 - item.getSize().x / 2, tile.getPosition().y + tile.getSize().y / 2 - item.getSize().y / 2 });

				// Create text for item quantity
				sf::Text count(std::to_string(item.getItemQuantity()), font, screenX / 100.f);
				count.setOutlineColor(sf::Color::Black);
				count.setOutlineThickness(2);
				count.setFillColor(sf::Color::White);

				// Insert temp objects into vectors
				allIcons.push_back(tile);
				items.push_back(item);
				itemCount.push_back(count);

			// Otherwise, create objects as normal
			} else {
				GameObject tile;
				tile.setFillColor(sf::Color::Green);
				tile.setSize({ screenX / 30.f, screenY / 20.f });
				tile.setPosition({ getPosition().x + screenX * j / 26 + screenX / 200, getPosition().y + screenY * i / 16 + screenY / 180.f });

				Item item(-1, 0);
				item.setPosition({ tile.getPosition().x + tile.getSize().x / 2 - item.getSize().x / 2, tile.getPosition().y + tile.getSize().y / 2 - item.getSize().y  / 2 });

				sf::Text count(std::to_string(item.getItemQuantity()), font, screenX / 100.f);
				count.setOutlineColor(sf::Color::Black);
				count.setOutlineThickness(2);
				count.setFillColor(sf::Color::White);

				allIcons.push_back(tile);
				items.push_back(item);
				itemCount.push_back(count);
			}
		}
	}
}

// Update function
// Update item textures, counters, and position in inventory
void Inventory::update(bool inInventory, int* menu, bool* isPaused) {

	// If gold > 100, move to win screen
	if (gold >= 2500) {
		*menu = 7;
		*isPaused = true;
		gold = 0;
	}

	// If item index is less than 0 while in inventory, reset to 0
	// Prevents out of bounds
	if (itemIndex < 0 && inInventory) {
		itemIndex = 0;

	// Stops player if they try to access inventory outside of hotbar while not in inventory menu
	} else if (itemIndex < allIcons.size() - columns && !inInventory) {
		allIcons[itemIndex].setFillColor(sf::Color::Green);
		itemIndex = allIcons.size() - columns;

	// If item index is greater than the size of the vector, reset to the final index
	} else if (itemIndex > allIcons.size() - 1) {
		itemIndex = allIcons.size() - 1;
	}

	// Highlight and update tile at current item index
	allIcons[itemIndex].setFillColor(sf::Color::Red);
	
	// For every item in items..
	for (int i = 0; i < items.size(); i++) {
		// If the item's id and quantity are the appropriate values, load the texture corresponding to the item's id
		if (items[i].getItemID() >= 0 && items[i].getItemQuantity() > 0) {
			items[i].setTexture(&textureList[items[i].getItemID()]);
		} else {
			items[i].resetItem();
		}

		// Set position of items according to their respective tile
		// Without this line, heldItem's change in position transfers to the item in inventory
		items[i].setPosition({ allIcons[i].getPosition().x + allIcons[i].getSize().x / 2 - items[i].getSize().x / 2, allIcons[i].getPosition().y + allIcons[i].getSize().y / 2 - items[i].getSize().y / 2});

		// Update counter text
		itemCount[i].setString(std::to_string(items[i].getItemQuantity()));

		// Set position of text
		itemCount[i].setPosition(allIcons[i].getPosition().x + allIcons[i].getSize().x - itemCount[i].getGlobalBounds().width * 1.1, allIcons[i].getPosition().y + allIcons[i].getSize().y - itemCount[i].getGlobalBounds().height * 1.4);
	}

	// If holding an item, move the heldItem object to be slightly above the item at the current index
	if (holdingItem) {
		heldItem.setPosition({ items[itemIndex].getPosition().x, items[itemIndex].getPosition().y - items[itemIndex].getSize().y/2});
	}
}

// handleInput function
// Allows player to navigate inventory and move items around
void Inventory::handleInput(bool inInventory) {
	// Input variables
	bool up = input->isPressed(sf::Keyboard::Up);
	bool down = input->isPressed(sf::Keyboard::Down);
	bool left = input->isPressed(sf::Keyboard::Left);
	bool right = input->isPressed(sf::Keyboard::Right);
	bool q = input->isPressed(sf::Keyboard::Q);
	bool e = input->isPressed(sf::Keyboard::E);

	// If up is pressed, decrease index by the number of columns (appear to move up in inventory)
	if (up) {
		allIcons[itemIndex].setFillColor(sf::Color::Green);
		itemIndex -= columns;
	}

	// If down is pressed, increase index by the number of columns
	if (down) {
		allIcons[itemIndex].setFillColor(sf::Color::Green);
		itemIndex += columns;
	}

	// If left is pressed, move left one space in inventory
	if (left && !right) {
		allIcons[itemIndex].setFillColor(sf::Color::Green);
		itemIndex -= 1;
	}

	// If right is pressed, move right one space in inventory
	if (right && !left) {
		allIcons[itemIndex].setFillColor(sf::Color::Green);
		itemIndex += 1;
	}

	// If q is pressed and the player is not currently holding an item, take entire stack of items from index
	if (q && items[itemIndex].getItemID() > -1 && !holdingItem) {
		// Set properties of held item equal to the item at index
		heldItem = items[itemIndex];

		// Set holdingItem to true
		holdingItem = true;

		// Clear properties of item at index
		items[itemIndex].resetItem();

	// Otherwise, is q is pressed and holding an item, add the heldItem to inventory slot if possible
	} else if (q && holdingItem) {
		// If inventory slot has an empty id or an id equal to the held item, call addToSlot
		if (items[itemIndex].getItemID() == -1) {
			addToSlot(false);
		} else if(items[itemIndex].getItemID() == heldItem.getItemID()) {
			addToSlot(true);
		// Otherwise, call swapHeld
		} else {
			swapHeld();
		}
	}

	// If e is pressed and the player is not holding an item, take one item from the stack from index
	if (e && items[itemIndex].getItemID() > -1 && !holdingItem) {
		// Set heldItem equal to item, set quantity of heldItem to 1
		heldItem = items[itemIndex];
		heldItem.setItemQuantity(1);
		holdingItem = true;

		// Decrease quantity
		items[itemIndex].setItemQuantity(items[itemIndex].getItemQuantity() -1);

	// If e is pressed and the heldItem's id is the same as the id of the item at index, take one item from the stack
	} else if (e && holdingItem) {
		if (items[itemIndex].getItemID() == heldItem.getItemID() && heldItem.getItemQuantity() < 10) {

			// Increase quantity of heldItem by one, decrease quantity of item at index by one
			heldItem.setItemQuantity(heldItem.getItemQuantity() + 1);
			items[itemIndex].setItemQuantity(items[itemIndex].getItemQuantity() - 1);

			// If the item's quantity becomes 0 through this method, clear properties of item
			if (items[itemIndex].getItemQuantity() == 0) {
				items[itemIndex].resetItem();
			}
		}
	}
}

// swapHeld function
// Swaps the held item with the item in the current inventory slot
void Inventory::swapHeld() {
	Item temp = heldItem;
	heldItem = items[itemIndex];
	items[itemIndex] = temp;
}

// addToSlot function
// Adds held item to inventory slot at current position
void Inventory::addToSlot(bool sameItem) {
	if(sameItem){
		// Loop that adds from heldItem to item at index until the item's quantity reaches 10
		while (items[itemIndex].getItemQuantity() < 10) {
			// Increase item quantity, decrease item quantity
			items[itemIndex].setItemQuantity(items[itemIndex].getItemQuantity() + 1);
			heldItem.setItemQuantity(heldItem.getItemQuantity() - 1);

			// If the quantity of heldItem become 0 during this process, break loop and clear properties of heldItem
			if (heldItem.getItemQuantity() == 0) {
				heldItem.resetItem();
				heldItem.setFillColor(sf::Color::Transparent);
				holdingItem = false;
				break;
			}
		}

	// Otherwise, set item at index equal to heldItem, then clear properties of heldItem
	} else {
		items[itemIndex] = heldItem;
		heldItem.resetItem();
		heldItem.setFillColor(sf::Color::Transparent);
		holdingItem = false;
	}
}

// Searches for a slot to add item to inventory, returns false otherwise
// Should drop item on ground if there is not space available (NOT IMPLEMENTED)
bool Inventory::addToInventory(int id, int amount) {
	// Loop to check space in hotbar first before checking rest of inventory
	for (int i = 0; i < columns; i++) {
		if (items[items.size() - columns + i].getItemID() == id && items[items.size() - columns + i].getItemQuantity() + amount <= 10) {
			items[items.size() - columns + i].setItemQuantity(items[items.size() - columns + i].getItemQuantity() + amount);
			return true;
		} else if (items[items.size() - columns + i].getItemID() == -1) {
			items[items.size() - columns + i].setItemQuantity(amount);
			items[items.size() - columns + i].setItemID(id);
			return true;
		}
	}

	// Check rest of inventory
	for (int i = 0; i < items.size() - columns; i++) {
		if (items[i].getItemID() == id && items[i].getItemQuantity() < 10) {
			items[i].setItemQuantity(items[i].getItemQuantity() + amount);
			return true;
		} else if (items[i].getItemID() == -1) {
			items[i].setItemQuantity(amount);
			items[i].setItemID(id);
			return true;
		}
	}

	// Return false if loops fail
	return false;
}

// getIndex function
// Returns current index in inventory
int Inventory::getIndex() {
	return itemIndex;
}

// getItemList function
// Returns list of items in inventory
std::vector<Item> Inventory::getItemList() {
	return items;
}

// removeFromSlot function
// Removes either one instance or all instances of the item at the given index
void Inventory::removeFromSlot(int index, bool removeAll) {
	if (removeAll) {
		items[index].resetItem();
	} else {
		items[index].setItemQuantity(items[index].getItemQuantity() - 1);
		if (items[index].getItemQuantity() == 0) {
			items[index].resetItem();
		}
	}
}

// loseGold function
// Decrease gold by a certain amount
void Inventory::loseGold(int amount) {
	if (gold - amount >= 0) {
		gold -= amount;
	} else {
		gold = 0;
	}

}

// gainGold function
// Increase gold by a certain amount
void Inventory::gainGold(int amount) {
	if (gold + amount <= 9999) {
		gold += amount;
	} else {
		gold = 9999;
	}
}

// getGold function
// Returns gold value
int Inventory::getGold() {
	return gold;
}

// find function
// Returns index of item stack matching specified ID & quantity, -1 otherwise
int Inventory::find(int id, int quantity) {
	int temp = 0;

	for (Item item : items) {
		if (item.getItemID() == id && item.getItemQuantity() == quantity) {
			return temp;
		}
		temp++;
	}

	return -1;
}

// drawInv function
// Draws all items to window, excluding the hotbar
void Inventory::drawInv() {
	// Draw each tile, item count, and item in inventory
	for (int i = 0; i < rows * columns; i++) {
		window->draw(allIcons[i]);
		if (items[i].getItemQuantity() > 0 && items[i].getItemID() >= 0) {
			window->draw(items[i]);
			window->draw(itemCount[i]);
		}
	}
}

// drawHotbar function
// Draws the hotbar items to window only
void Inventory::drawHotbar() {
	// Draw hotbar shape
	window->draw(hotbar);

	// Draw each tile, item count, and item in hotbar
	for (int i = 0; i < columns; i++) {
		window->draw(allIcons[allIcons.size() - columns + i]);
		if (items[items.size() - columns + i].getItemQuantity() > 0 && items[items.size() - columns + i].getItemID() >= 0) {
			window->draw(items[items.size() - columns + i]);
			window->draw(itemCount[itemCount.size() - columns + i]);
		}
	}
	// If holding an item, render heldItem
	if (holdingItem) {
		window->draw(heldItem);
	}
}

// drawOverlay function
// Draws screen overlay to window
void Inventory::drawOverlay() {
	// Draw overlay
	window->draw(overlay);
}