#pragma once
#include "Interactable.h"
#include "Player.h"

// Parent Enemy Class
class Enemy : public Interactable {
	protected:
		// Texture
		sf::Texture texture;

		// Movement variables
		bool hasAggro;
		sf::Vector2f speed;
		sf::Vector2f direction;

		sf::Vector2f distanceFrom;

		// Detection
		GameObject innerDetectionCircle;
		GameObject outerDetectionCircle;

	public:
		// Constructor
		Enemy();

		// Detect player and determine aggro
		void detectPlayer(Player* p, float dt);
};

