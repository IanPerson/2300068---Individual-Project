#pragma once
#include "Interactable.h"
#include "Button.h"
#include "Cursor.h"
#include "Item.h"
#include <vector>

class Inventory : public Interactable {
	private:
		// Get screen resolution
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Stores number of rows & columns in inventory
		int rows; int columns;

		// Stores currently held gold
		int gold;

		// Current position in inventory
		int itemIndex;
		
		// Vectors for item tiles in inventory
		std::vector<GameObject> allIcons;
		std::vector<Item> items;
		std::vector<sf::Text> itemCount;

		// Font
		sf::Font font;

		// Item object to store item being moved around
		Item heldItem{ -1 };

		// Determines if player is currently moving an item
		bool holdingItem;

		// Vector for item textures
		std::vector<sf::Texture> textureList;

		// Shape for hotbar
		sf::RectangleShape hotbar;

		// Shape to darken screen when in inventory
		sf::RectangleShape overlay;

	public:
		// Constructor
		Inventory(int rows, int columns);

		// Initialize function
		// Initializes most values and objects within class
		void initialize();

		// drawInv function
		// Draws all items to window, excluding the hotbar
		void drawInv();

		// drawHotbar function
		// Draws the hotbar items to window only
		void drawHotbar();

		// drawOverlay function
		// Draws screen overlay to window
		void drawOverlay();

		// Update function
		// Update item textures, counters, and position in inventory
		void update(bool inInventory, int* menu, bool* isPaused);

		// handleInput function
		// Allows player to navigate inventory and move items around
		void handleInput(bool inInventory);

		// addToSlot function
		// Adds held item to inventory slot at current position
		void addToSlot(bool sameItem);

		// removeFromSlot function
		// Removes either one instance or all instances of the item at the given index
		void removeFromSlot(int index, bool removeAll);

		// swapHeld function
		// Swaps the held item with the item in the current inventory slot
		void swapHeld();
		
		// addToInventory function
		// Adds item to inventory if space is found
		bool addToInventory(int id, int amount);

		// find function
		// Returns index of item stack matching specified ID & quantity, -1 otherwise
		int find(int id, int quantity);

		// loseGold function
		// Decrease gold by a certain amount
		void loseGold(int amount);

		// gainGold function
		// Increase gold by a certain amount
		void gainGold(int amount);

		// getGold function
		// Returns gold value
		int getGold();

		// getIndex function
		// Returns current index in inventory
		int getIndex();

		// getItemList function
		// Returns list of items in inventory
		std::vector<Item> getItemList();
};

