#pragma once
#include "Enemy.h"
#include "Lantern.h"
#include "Framework/Animation.h"

class Chaser : public Enemy {
	private:
		// Attacking Variables
		float attackCooldown;
		float attackDuration;

		// Animation
		Animation idle;

		// Movement
		float moveCooldown;
		float moveDuration;
		float fleeDuration;
		sf::Vector2f dashSpeed;

		// Enemy States
		bool isAttacking;
		bool fleeing;
		bool playerHit;

		
		GameObject hitBox;
		int currentCollisions = 0;

		int currentCollisions = 0;

	public:
		// Constructor
		Chaser(sf::Vector2f pos);

		// Detect player and nearby lamps
		void update(Player* p, float dt, std::vector<Lantern*> l);

		// Handle player input when they are holding a lantern
		void handleInput(Player* p, Input* input, bool* k);

		// Attack the player
		void attack(Player* p, float dt);

		// Render to window
		void draw(sf::RenderWindow* w);

		// Collision
		void collisionResponse(GameObject* o);
};

