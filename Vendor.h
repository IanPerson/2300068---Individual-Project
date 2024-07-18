#pragma once
#include "Interactable.h"
#include "Inventory.h"
#include "Wall.h"

class Vendor : public Interactable {
	private:
		// Get screen resolution
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Vectors for items & item selection
		std::vector<Item> stock;
		std::vector<Item> sellStock;
		std::vector<bool> isSelected;

		// Player detection
		sf::FloatRect detectionBox;

		// Determines item selected & position in menu
		int pageCount;
		int currentPage;
		int currentRow;
		int currentIndex;

		int interact = 1;

		// Determines pool of items held in stock
		int itemPool;

		// Display Variables
		sf::RectangleShape menuBody;
		sf::RectangleShape menuBody2;
		sf::RectangleShape confirm;
		std::vector<sf::RectangleShape> itemTiles;

		// Text variables
		std::string itemNames[15] = {"Wheat Seeds","Potato Seeds","Carrot Seeds","Onion Seeds","Wheat","Potato","Carrot","Onion","Egg","","","","","",""};
		std::vector<sf::Text> itemText;
		sf::RectangleShape goldBox;
		sf::Text goldText;
		sf::Font font;

		// Vendor & item textures
		sf::Texture vendorTexture;
		std::vector<sf::Texture> itemTextures;

		// Buy & sell prices
		int sellPrice[9] = { 15, 20, 25, 30, 35, 40, 45, 50, 55 };
		int buyPrice[9] = { 30, 40, 50, 60, 70, 80, 90, 100, 110 };

		// Determines if player is trading with this vendor
		bool isTrading;

	public:
		// Constructor
		Vendor(int pool);

		// Update function
		// Checks player detection & updates item counts for sell stock
		void update(Player* p);

		// handleInput function
		// Allows player to start trading with vendor
		// When trading with vendor, enables player to navigate menus and buy/sell items
		void handleInput(Inventory* inv);

		// collisionCheck function
		// Stops player when colliding with object 
		void collisionCheck(Player* p, sf::View v, float dt);

		// Restock function
		// Refreshes the inventory according to the specified pool
		void restock(int pool);

		// refreshSell function
		// Refreshes the player's side of the vendor menu with updated inventory
		void refreshSell(Inventory* inv);

		// Buy function
		// If player has enough gold, takes item from stock and adds it to inventory then removes gold
		void buy(int id, Inventory* inv, bool sellAll);

		// Sell function
		// Removes item from inventory and adds gold to inventory
		void sell(int id, Inventory* inv, bool sellAll);

		// drawStock function
		// Draws vendor menu in specified window
		void drawStock(sf::RenderWindow* w);

		// tradeState function
		// Returns value of isTrading
		bool tradeState();
};