#include "Vendor.h"

// Constructor
Vendor::Vendor(int pool) {
	// Set Properties
	setSize({ 240,176 });
	setPosition({ 3000,2450 });

	// Set collision
	collisionBox.width = getSize().x - 30;
	collisionBox.height = getSize().y - 30;
	collisionBox.left = 15;
	collisionBox.top = 15;
	setCollisionBox(collisionBox);

	// Set detection
	detectionBox.left = getPosition().x - 50;
	detectionBox.top = getPosition().y - 50;
	detectionBox.width = getSize().x + 100;
	detectionBox.height = getSize().y + 100;

	// Starting positions in menu
	pageCount = 0;
	currentIndex = 0;

	// Load texture
	vendorTexture.loadFromFile("gfx/Vendor.png");
	setTexture(&vendorTexture);

	// Box properties for sell stock
	menuBody.setSize({ sf::VideoMode::getDesktopMode().width * 3 / 8.f, sf::VideoMode::getDesktopMode().height * 5 / 8.f });
	menuBody.setPosition({ sf::VideoMode::getDesktopMode().width / 15.f, sf::VideoMode::getDesktopMode().height / 10.f });
	menuBody.setOutlineColor(sf::Color(120, 80, 60, 255));
	menuBody.setFillColor(sf::Color(180, 140, 120, 255));
	menuBody.setOutlineThickness(5);

	// Box properties for buy stock
	menuBody2.setSize({ menuBody.getSize().x, menuBody.getSize().y});
	menuBody2.setPosition({ sf::VideoMode::getDesktopMode().width - sf::VideoMode::getDesktopMode().width / 15.f - menuBody2.getSize().x, menuBody.getPosition().y});
	menuBody2.setOutlineColor(sf::Color(120, 80, 60, 255));
	menuBody2.setFillColor(sf::Color(180, 140, 120, 255));
	menuBody2.setOutlineThickness(5);

	// Confirm purchases box properties
	confirm.setSize({ sf::VideoMode::getDesktopMode().width / 3.f, sf::VideoMode::getDesktopMode().height / 9.f });
	confirm.setPosition({ sf::VideoMode::getDesktopMode().width / 3.f, sf::VideoMode::getDesktopMode().height - confirm.getSize().y * 3/2.f});
	confirm.setOutlineColor(sf::Color(120, 80, 60, 255));
	confirm.setFillColor(sf::Color(180, 140, 120, 255));
	confirm.setOutlineThickness(5);

	// Load font
	font.loadFromFile("font/Daydream.ttf");

	// Load item tiles
	// Rows
	for (int i = 0; i < 4; i++) {
		// Columns
		for (int j = 0; j < 2; j++) {
			// Create variables
			sf::Text name;
			sf::RectangleShape tile;

			// Set box properties
			tile.setSize({ sf::VideoMode::getDesktopMode().width / 3.f, sf::VideoMode::getDesktopMode().height / 8.f });
			if (j == 0) {
				tile.setPosition({ menuBody.getPosition().x + menuBody.getSize().x / 2 - tile.getSize().x / 2, menuBody.getPosition().y + menuBody.getSize().y/15.f + i * tile.getSize().y * 9/8.f});
			} else {
				tile.setPosition({ menuBody2.getPosition().x + menuBody2.getSize().x / 2 - tile.getSize().x / 2, menuBody2.getPosition().y + menuBody2.getSize().y / 15.f + i * tile.getSize().y * 9 / 8.f });
			}
			tile.setFillColor(sf::Color(220, 180, 160, 255));
			tile.setOutlineColor(sf::Color(160, 120, 100, 255));
			tile.setOutlineThickness(5);

			// Set text properties
			name.setFont(font);
			name.setCharacterSize(screenX / 75);
			name.setOutlineColor(sf::Color::Black);
			name.setOutlineThickness(2);
			name.setFillColor(sf::Color::White);
			name.setPosition(tile.getPosition().x + tile.getSize().x / 4, tile.getPosition().y + tile.getSize().y / 3);

			// Add variables to vectors
			itemTiles.push_back(tile);
			itemText.push_back(name);
		}
	}

	// Create tile to exit vendor menu & add to tiles vector
	sf::RectangleShape confirmTile;
	confirmTile.setSize({ confirm.getSize().x * 7/8.f, confirm.getSize().y * 7/8.f});
	confirmTile.setPosition({ confirm.getPosition().x + confirm.getSize().x/2 - confirmTile.getSize().x /2, confirm.getPosition().y + confirm.getSize().y / 2 - confirmTile.getSize().y / 2 });
	confirmTile.setFillColor(sf::Color(220, 180, 160, 255));
	confirmTile.setOutlineColor(sf::Color(160, 120, 100, 255));
	confirmTile.setOutlineThickness(5);
	itemTiles.push_back(confirmTile);

	sf::Text confirmText;
	confirmText.setFont(font);
	confirmText.setString("Confirm");
	confirmText.setCharacterSize(screenX / 75);
	confirmText.setOutlineColor(sf::Color::Black);
	confirmText.setOutlineThickness(2);
	confirmText.setFillColor(sf::Color::White);
	confirmText.setPosition(confirmTile.getPosition().x + confirmTile.getSize().x / 4, confirmTile.getPosition().y + confirmTile.getSize().y / 3);
	itemText.push_back(confirmText);

	// Set properties for gold box
	goldBox.setSize({ sf::VideoMode::getDesktopMode().width / 4.f, sf::VideoMode::getDesktopMode().height / 8.f });
	goldBox.setPosition({ menuBody.getPosition().x, confirm.getPosition().y + confirm.getSize().y - goldBox.getSize().y});
	goldBox.setOutlineColor(sf::Color(120, 80, 60, 255));
	goldBox.setFillColor(sf::Color(180, 140, 120, 255));
	goldBox.setOutlineThickness(5);

	// Set properties for gold text
	goldText.setFont(font);
	goldText.setCharacterSize(screenX / 75);
	goldText.setOutlineColor(sf::Color::Black);
	goldText.setOutlineThickness(2);
	goldText.setFillColor(sf::Color::White);
	goldText.setPosition({goldBox.getPosition().x + 50, goldBox.getPosition().y + 50});

	// Load item textures
	for (int i = 1; i < 10; i++) {
		sf::Texture texture;
		texture.loadFromFile("gfx/Items/" + std::to_string(i) + ".png");
		itemTextures.push_back(texture);
	}

	// Set vendor item pool
	itemPool = pool;

	// Restock
	restock(itemPool);

	// Default isTrading to false
	isTrading = false;
}

// Update function
// Checks player detection & updates item counts for sell stock
void Vendor::update(Player* p) {
	// Highlight object if player collides with detection box
	if (p->getCollisionBox().intersects(detectionBox)) {
		setFillColor(sf::Color(255,205,205,255));
		keyboardInteract = true;

		if (!isTrading) {
			p->setPrompt("F to shop");
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

	// Highlight item tile at current position in menu
	itemTiles[currentIndex].setFillColor(sf::Color(255,205,205,255));

	// Update item counters in menu
	for (int i = 0; i < 4; i++) {
		if (4 * currentPage + i < sellStock.size()) {
			itemText[i * 2].setString(itemNames[sellStock[4 * currentPage + i].getItemID()] + "   x   " + std::to_string(sellStock[4 * currentPage + i].getItemQuantity()));
		} else {
			itemText[i * 2].setString("");
		}

		if (i < stock.size()) {
			itemText[i * 2 + 1].setString(itemNames[stock[i].getItemID()] + "   x   " + std::to_string(stock[i].getItemQuantity()));
		}
		else {
			itemText[i * 2 + 1].setString("");
		}
	}
}

// collisionCheck function
// Stops player when colliding with object - see collisionResponse for more info
void Vendor::collisionCheck(Player* p, sf::View v, float dt) {
	if (p->getCollisionBox().intersects(getCollisionBox())) {
		p->collisionResponse(this);
	}
}

// handleInput function
// Allows player to start trading with vendor
// When trading with vendor, enables player to navigate menus and buy/sell items
void Vendor::handleInput(Inventory* inv) {
	// Menu Navigation
	bool up = input->isPressed(sf::Keyboard::Up);
	bool down = input->isPressed(sf::Keyboard::Down);
	bool left = input->isPressed(sf::Keyboard::Left);
	bool right = input->isPressed(sf::Keyboard::Right);

	// Switch inventory page
	bool e = input->isPressed(sf::Keyboard::E);

	// Buy/Sell & Exit menu
	bool f = input->isPressed(sf::Keyboard::F);

	// Restock vendor
	bool r = input->isPressed(sf::Keyboard::R);

	// Buy/Sell All
	bool shift = input->isKeyDown(sf::Keyboard::LShift) || input->isKeyDown(sf::Keyboard::RShift);

	// If not already trading, check overworld input
	// Otherwise, check menu input
	if (!isTrading) {
		// If f is pressed, initiate trading
		if (f && keyboardInteract) {
			isTrading = true;
			refreshSell(inv);
		}
	} else {
		// If the confirm tile is currently selected, exit trading 
		if (f && currentIndex == 8) {
			isTrading = false;
		}

		// If up is pressed and new index is within range of tiles vector, move up in menu 
		if (up && currentIndex - 2 >= 0) {
			itemTiles[currentIndex].setFillColor(sf::Color(220, 180, 160, 255));
			currentIndex -= 2;
			currentRow -= 1;
		}

		// If down is pressed and new index is within range of tiles vector, move down in menu 
		if (down && currentIndex + 2 <= 8) {
			itemTiles[currentIndex].setFillColor(sf::Color(220, 180, 160, 255));
			currentIndex += 2;
			currentRow += 1;
		// This check prevents out of range error when trying to select confirm tile
		} else if (down && currentIndex + 1 <= 8) {
			itemTiles[currentIndex].setFillColor(sf::Color(220, 180, 160, 255));
			currentIndex += 1;
			currentRow += 1;
		}

		// If left is pressed and player is on the right menu, move to left menu
		if (left && currentIndex % 2 == 1 && currentIndex - 1 >= 0) {
			itemTiles[currentIndex].setFillColor(sf::Color(220, 180, 160, 255));
			currentIndex -= 1;
		}

		// If right is pressed and player is on the left menu, move to right menu
		if (right && currentIndex % 2 == 0 && currentIndex + 1 < 8) {
			itemTiles[currentIndex].setFillColor(sf::Color(220, 180, 160, 255));
			currentIndex += 1;
		}
		
		// If e is pressed, show new page of inventory in sell stock
		if (e) {
			if (currentPage < pageCount) {
				currentPage++;
			} else {
				currentPage = 0;
			}
		}

		// If player is selecting an item from sell stock and presses f, sell selected item
		// Sells all of selected item if shift is held
		if (currentIndex % 2 == 0) {
			if (shift && f && 4 * currentPage + currentRow < sellStock.size()) {
				sell(4 * currentPage + currentRow, inv, true);
			} else if (f && 4 * currentPage + currentRow < sellStock.size()) {
				sell(4 * currentPage + currentRow, inv, false);
			}
		// If player is selecting an item from buy stock and presses f, buy selected item
		// If shift is held when f is pressed, buys as many of selected item as possible depending on held gold
		} else if (currentIndex != 8) {
			if (shift && f) {
				buy(currentRow, inv, true);
			} else if (f) {
				buy(currentRow, inv, false);
			}
		}
	}
}

// Restock function
// Refreshes the inventory according to the specified pool
void Vendor::restock(int pool) {
	// Clear current stock
	stock.clear();

	// Add items to stock based on pool
	switch (pool) {

	// Selling only (shouldn't need to be used)
	case 1:
		break;

	// Seeds
	case 2:
		// Load items
		for (int i = 0; i < 4; i++) {
			Item item(i, 5);
			item.setPosition({ itemTiles[i * 2 + 1].getPosition().x + itemTiles[i * 2 + 1].getSize().x / 8, itemTiles[i * 2 + 1].getPosition().y + itemTiles[i * 2 + 1].getSize().y / 3 });
			stock.push_back(item);
		}
		break;
	// Food
	case 3:
		// Load items
		for (int i = 0; i < 4; i++) {
			Item item(i + 4, 5);
			item.setPosition({ itemTiles[i * 2 + 1].getPosition().x + itemTiles[i * 2 + 1].getSize().x / 8, itemTiles[i * 2 + 1].getPosition().y + itemTiles[i * 2 + 1].getSize().y / 3 });
			stock.push_back(item);
		}
		break;
	}

	// Set item textures
	for (int i = 0; i < stock.size(); i++) {
		stock[i].setTexture(&itemTextures[stock[i].getItemID()]);
	}
}

// refreshSell function
// Refreshes the player's side of the vendor menu with updated inventory
void Vendor::refreshSell(Inventory* inv) {
	// Temp variable determining row in menu
	int row = 0;

	// Clear current sell stock
	sellStock.clear();

	// Loop through inventory and add all items to sell stock
	for (int i = 0; i < inv->getItemList().size(); i++) {
		// If item at index has a valid ID
		if (inv->getItemList()[i].getItemID() != -1) {
			// Create item identical to item at index
			Item temp = inv->getItemList()[i];

			// Depending on result of tempCount % 4, position item in a certain row of menu
			// First row
			if (row % 4 == 0) {
				temp.setPosition({ itemTiles[0].getPosition().x + itemTiles[0].getSize().x / 8, itemTiles[0].getPosition().y + itemTiles[0].getSize().y / 3});
			}
			// Second row
			else if (row % 4 == 1) {
				temp.setPosition({ itemTiles[2].getPosition().x + itemTiles[2].getSize().x / 8, itemTiles[2].getPosition().y + itemTiles[2].getSize().y / 3 });
			}
			// Third row
			else if (row % 4 == 2) {
				temp.setPosition({ itemTiles[4].getPosition().x + itemTiles[4].getSize().x / 8, itemTiles[4].getPosition().y + itemTiles[4].getSize().y / 3 });
			}
			// Fourth row
			else if (row % 4 == 3) {
				temp.setPosition({ itemTiles[6].getPosition().x + itemTiles[6].getSize().x / 8, itemTiles[6].getPosition().y + itemTiles[6].getSize().y / 3 });
			}
			// Set item texture & push into vector
			temp.setTexture(&itemTextures[temp.getItemID()]);
			sellStock.push_back(temp);
			// Increase row
			row++;
		}
	}

	// Create pages which each hold up to four items
	pageCount = (sellStock.size() / 4);

	// Update gold
	goldText.setString("Gold: " + std::to_string(inv->getGold()));
}

// Buy function
// If player has enough gold, takes item from stock and adds it to inventory then removes gold
void Vendor::buy(int id, Inventory* inv, bool buyAll) {
	// If buyAll is true, buys selected item while player has enough gold and vendor has enough in stock 
	if (buyAll) {
		// Continuously buy items while vendor has stock
		while ((inv->getGold() > buyPrice[stock[id].getItemID()]) && (stock[id].getItemQuantity() > 0)) {
			// Lose gold equal to buying price
			inv->loseGold(buyPrice[stock[id].getItemID()]);
			// Add 1 of item to inventory
			inv->addToInventory(stock[id].getItemID(), 1);
			// If quantity of stock is greater than 0, lower quantity by 1
			if (stock[id].getItemQuantity() > 0) {
				stock[id].setItemQuantity(stock[id].getItemQuantity() - 1);
			}
		}
	// Otherwise, buy one instance of the selected item if player has enough gold and vendor has enough in stock
	} else if (inv->getGold() > buyPrice[stock[id].getItemID()] && (stock[id].getItemQuantity() > 0)) {
		// Lose gold equal to buying price
		inv->loseGold(buyPrice[stock[id].getItemID()]);
		// Add 1 of item to inventory
		inv->addToInventory(stock[id].getItemID(), 1);
		// If quantity of stock is greater than 0, lower quantity by 1
		if (stock[id].getItemQuantity() > 0) {
			stock[id].setItemQuantity(stock[id].getItemQuantity() - 1);
		}
	}
	
	// Refresh sell stock once function finishes
	refreshSell(inv);
}

// Sell function
// Removes item from inventory and adds gold to inventory
void Vendor::sell(int id, Inventory* inv, bool sellAll) {
	// If sell all is true, remove all items from selected stack and gain gold for every item sold
	if (sellAll) {
		inv->gainGold(sellPrice[sellStock[id].getItemID()] * sellStock[id].getItemQuantity());
		inv->removeFromSlot(inv->find(sellStock[id].getItemID(), sellStock[id].getItemQuantity()), true);
	// Otherwise, sell one instance of selected item and gain gold
	} else {
		inv->gainGold(sellPrice[sellStock[id].getItemID()]);
		inv->removeFromSlot(inv->find(sellStock[id].getItemID(), sellStock[id].getItemQuantity()), false);
	}

	// Refresh sell stock once function finishes
	refreshSell(inv);
}

// drawStock function
// Draws vendor menu in specified window
void Vendor::drawStock(sf::RenderWindow* w) {
	// Draw static menu objects
	w->draw(menuBody);
	w->draw(menuBody2);
	w->draw(confirm);
	w->draw(goldBox);
	w->draw(goldText);

	// Draw item tiles
	for (int i = 0; i < itemTiles.size(); i++) {
		w->draw(itemTiles[i]);
	}

	// Draw text
	for (int i = 0; i < itemText.size(); i++) {
		w->draw(itemText[i]);
	}

	// Draws stock
	// Draws sell stock for the current page
	for (int i = 0; i < 4; i++) {
		w->draw(stock[i]);

		// Checks if page has less than 4 items before drawing
		if (4 * currentPage + i < sellStock.size()) {
			w->draw(sellStock[4 * currentPage + i]);
		}
	}
}

// tradeState function
// Returns value of isTrading
bool Vendor::tradeState() {
	return isTrading;
}