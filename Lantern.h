#pragma once
#include "Interactable.h"

class Lantern : public Interactable {
	private:
		// General properties
		sf::RectangleShape light;
		sf::Vector2f position;
		sf::Vector2f size;

		// Sprites and animation
		sf::Texture animations[2];
		Animation lanternAnimation;
		sf::RectangleShape lanternSprite;

		// Gameplay
		int interact = 1;
		float lightCountdown;
		bool isActive;
		bool used;

	public:
		// Constructor
		Lantern(sf::Vector2f s, sf::Vector2f pos);

		// Detect player and fizzle out after a certain amount of time
		void update(Player* p, float dt);

		// Handle input for relighting and taking lanterns
		void handleInput(Player* p, Input* i);

		// Render lantern sprite
		void drawLantern(sf::RenderWindow* w);

		// Set state variables
		// Active - determines if lantern is lit
		// Used - determines if player has taken lantern to hold
		void setActive(bool b);
		void setUsed(bool b);
		bool getActive();
		bool getUsed();

		// Reset lantern
		void reset();

		// Get lantern size
		sf::Vector2f getOriginalSize();
};

