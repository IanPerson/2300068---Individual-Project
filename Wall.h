#pragma once
#include "Framework/GameObject.h"
#include "Player.h"
#include "Chaser.h"

class Wall : public GameObject {
	private:
		// Get screen resolution
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Object & collision properties
		sf::Vector2f position;
		sf::Vector2f collisionP;
		sf::Vector2f collisionSize;

	public:
		// Constructors and Deconstructors
		Wall();
		Wall(sf::Vector2f position, sf::Vector2f collisionBoxPos, sf::Vector2f size);
		~Wall();

		// Update function
		// Only check collision if within a certain distance of player
		// If the player collides with wall, check the direction of impact and push opposite direction
		void update(Player* p, sf::View v, float dt);
		void update(Chaser* p, sf::View v, float dt);

};

