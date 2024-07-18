#pragma once
#include "Framework/GameObject.h"
class Item : public GameObject {
	private:
		// # of item held
		int itemQuantity;

		// ID of item held
		int itemID;

		// Item texture
		sf::Texture itemTexture;

	public:
		// Constructors & deconstructor
		Item(int id);
		Item(int id, int itemQuantity);
		~Item();
		
		// loadTexture function
		// If item's ID is valid, load texture from specified location
		void loadTexture(std::string fileLocation);

		// getItemID function
		// Returns item's ID
		int getItemID();

		// getItemQuantity function
		// Returns the quantity of an item this object carries
		int getItemQuantity();

		// setItemID function
		// Sets the item's ID to the specified value
		void setItemID(int id);

		// setItemQuantity function
		// Sets the quantity of the item held to the specified amount
		void setItemQuantity(int _itemQuantity);

		// resetItem function
		// Resets item to hold empty values
		void resetItem();
};

