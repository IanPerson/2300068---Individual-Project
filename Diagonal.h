#pragma once
#include "Framework/GameObject.h"
#include <vector>
#include "Player.h"

class Diagonal : public GameObject {
	private:
		// Variables
		int screenW = sf::VideoMode::getDesktopMode().width;
		int screenH = sf::VideoMode::getDesktopMode().height;

		// Direction of movement for creating collision
		sf::Vector2f direction;

		// Used to evenly space collision objects during creation
		sf::Vector2f directionNormalise;

		// Points for creating collision
		sf::Vector2f point1;
		sf::Vector2f point2;

		// List of collision objects
		std::vector<sf::FloatRect> collisionLine;

	public:
		// Position - Final position of object/sprite
		// p1 - Starting point for creating collision
		// p2 - End point for creating collision
		Diagonal(sf::Vector2f position, sf::Vector2f p1, sf::Vector2f p2);
		~Diagonal();

		// Collision check function
		void update(Player* p, sf::View v, float dt);
};

