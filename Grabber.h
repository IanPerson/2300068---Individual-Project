#pragma once
#include "Enemy.h"
class Grabber : public Enemy {
	private:
		// Arm Properties
		GameObject grabBox;
		GameObject arm;
		GameObject maxRange;
		sf::Vector2f lineDistance;
		sf::Vector2f grabDistance;
		sf::Vector2f armDistance;

		// Arm movement
		float grabAngle = 0;
		float lineAngle = 0;
		float armAngle = 0;

		// Limit arm range
		bool inBounds;
		sf::Vector2f circleEdge;

		// Arm textures
		sf::Texture limbTexture;
		sf::Texture handTexture;

		// Misc
		float retractSpeed;
		int keyPressCount;
		int currentCollisions = 0;
		float attackCooldown = 1.5;

		sf::Vertex line[2] = {
			sf::Vertex(sf::Vector2f(0,0)),
			sf::Vertex(sf::Vector2f(0,0))
		};
		

	public:
		// Constructor
		Grabber(sf::Vector2f pos);

		// Detect player and act accordingly
		void update(Player* p, float dt);

		// Handle input when trapping player
		void handleInput(Player* p, Input* input, bool* k);

		// Retract arm
		void retract(Player* p, bool grabbing, float dt);

		// Render to window
		void draw(sf::RenderWindow* w);

		// Collision
		void collisionResponse(GameObject* o);

		// Set arm textures
		void setTextures(sf::Texture* arm, sf::Texture* hand);
};

