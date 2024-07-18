#pragma once
#include "Chaser.h"
#include "Grabber.h"
#include <vector>
#include <variant>
#include "Framework/AudioManager.h"

class EnemyManager : public GameObject {
	private:
		// Vector for spawned enemies
		std::vector<Enemy*> enemies;

		// Boundary for spawned enemies
		sf::Vector2f position;
		sf::Vector2f borders;

	public:
		// Constructor and deconstructor
		EnemyManager(sf::Vector2f b, sf::Vector2f p);
		~EnemyManager();

		// Randomly spawns enemies during the night and calls their update functions
		void update(float dt, int time, Player* p, int currentRoom, AudioManager* audio, std::vector<Lantern*> l);

		// Calls all enemy handleInput functions
		void handleInput(Player* p, Input* input, int currentRoom);

		// Render enemies to window
		void drawEnemies(sf::RenderWindow* w);
};

