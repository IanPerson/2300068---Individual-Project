#include "Chest.h"

Chest::Chest(int p, sf::Vector2f pos) {
	// General properties
	pool = p;
	position = pos;
	setPosition(pos);
	setSize({100,100});

	// Collision
	texture.loadFromFile("gfx/Crate.png");
	setTexture(&texture);

	collisionBox.height = 100;
	collisionBox.width = 100;
	setCollisionBox(collisionBox);

	// Detection
	detection.width = getSize().x + 100;
	detection.height = getSize().y + 100;
	detection.left = pos.x - 50;
	detection.top = pos.y - 50;

	// Generate loot
	spawnLoot();
}

// spawnLoot function
// Clears loot currently in chest and generates new loot
void Chest::spawnLoot() {
	loot.clear();
	srand(time(0));
	int itemCount = rand() % 3 + 1;

	// Generate different items depending on chest's pool
	if (rand() % 2 == 1) {
		switch (pool) {
			case 1:
				for (int i = 0; i < itemCount; i++) {
					loot.push_back({ rand() % 4 + 1, rand() % 2 + 1 });
				}
				break;
			case 2:
				for (int i = 0; i < itemCount; i++) {
					loot.push_back({ rand() % 4 + 5, rand() % 2 + 1 });
				}
				break;
			case 3:
				for (int i = 0; i < itemCount; i++) {
					loot.push_back({ rand() % 8 + 1, rand() % 2 + 1 });
				}
				break;
		}
	}
}

// Update function
// Detect player for collision
// Enable interaction if loot is available
void Chest::update(Player* p) {
	if (loot.size() > 0) {
	if (p->getCollisionBox().intersects(detection)) {
			setFillColor(sf::Color(255, 50, 50, 255));
		keyboardInteract = true;
			if (interact == 1) {
				p->setPrompt("F to loot");
				p->setInteract(p->getInteract() + 1);
				interact -= 1;
			}
		}
		else {
			setFillColor(sf::Color(255, 255, 255, 255));
			keyboardInteract = false;
			if (interact == 0) {
				p->setPrompt("");
				p->setInteract(p->getInteract() - 1);
				interact += 1;
			}
		}
	} else {
		setFillColor(sf::Color(255, 255, 255, 255));
	}

	if (p->getCollisionBox().intersects(getCollisionBox())) {
		p->collisionResponse(this);
	}
}

// handleInput function
// Add items in chest to player inventory when interacted with, then clear items from chest
void Chest::handleInput(Input* input, Player* p, Inventory* i) {
	if (input->isPressed(sf::Keyboard::F) && keyboardInteract) {
		for (auto arr : loot) {
			i->addToInventory(arr[0], arr[1]);
		}
		loot.clear();
	}
}