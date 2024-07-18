#include "EnemyManager.h"

EnemyManager::EnemyManager(sf::Vector2f b, sf::Vector2f p) {
	borders = b;
	position = p;
}

EnemyManager::~EnemyManager() {

}

void EnemyManager::update(float dt, int time, Player* p, int currentRoom, AudioManager* audio, std::vector<Lantern*> l) {
	int random = rand() % 100;
	if (time <= 1 && random >= 96) {
		if (random % 2 == 0) {
			enemies.push_back(new Chaser({ (float)(rand() % (int)borders.x) + position.x, (float)(rand() % (int)borders.y) + position.y }));
		} else {
			enemies.push_back(new Grabber({ (float)(rand() % (int)borders.x) + position.x, (float)(rand() % (int)borders.y) + position.y }));
			enemies.back()->setInput(input);
		}
	}

	if (enemies.size() > 0) {
		if (rand() % 1000 > 990) {
			switch (rand() % 4) {
				case 0:
					audio->playSoundbyName("Growl1");
					break;
				case 1:
					audio->playSoundbyName("Growl2");
					break;
				case 2:
					audio->playSoundbyName("Ambience1");
					break;
				case 3:
					audio->playSoundbyName("Ambience2");
					break;
			}
		}
	}

	if (time >= 3) {
		for (auto& e : enemies) {
			delete e;
		}
		enemies.clear();
	}

	if (currentRoom == 0) {
		for (auto e : enemies) {
			if (typeid(*e) == typeid(Grabber)) {
				dynamic_cast<Grabber*>(e)->update(p, dt);
			} else {
				dynamic_cast<Chaser*>(e)->update(p, dt, l);
			}
		}
	}
}

void EnemyManager::handleInput(Player* p, Input* input, int currentRoom) {
	if (currentRoom == 0) {
		for (auto e : enemies) {
			bool killed = false;
			if (typeid(*e) == typeid(Grabber)) {
				dynamic_cast<Grabber*>(e)->handleInput(p, input, &killed);
			} else {
				dynamic_cast<Chaser*>(e)->handleInput(p, input, &killed);
			}
			if (killed) {
				enemies.erase(std::find(enemies.begin(), enemies.end(), e));
			}
		}
	}
}

void EnemyManager::drawEnemies(sf::RenderWindow* w) {
	for (auto& e : enemies) {
		if (typeid(*e) == typeid(Grabber)) {
			dynamic_cast<Grabber*>(e)->draw(w);
		} else {
			w->draw(*e);
		}
	}
}