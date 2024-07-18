#pragma once
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include "Player.h"

class Interactable : public GameObject, public Input {
	private:
		// Interact function
		// Test function only - use specialized functions in derived classes
		void interact();

	protected:
		// Variables

		// Enables interaction Using keyboard and mouse
		bool keyboardInteract;
		bool mouseInteract;
		sf::Vector2f position;
		sf::Vector2f size;

	public:
		// Constructors & deconstructor
		Interactable();
		Interactable(float x, float y);
		~Interactable();

		// Functions

		// interactCheck function
		// int type -> determines which interact variable to check (0 for keyboard, 1 for mouse)
		// Returns value of checked interact variable
		bool interactCheck(int type);

		// Update function
		// Highlight object and enable keyboard interaction if player collides with object
		void update(Player p);

		// handleInput function
		// Check input while interaction is enabled
		void handleInput(float dt);
};