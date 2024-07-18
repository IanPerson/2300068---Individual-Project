#include "DayCycle.h"
#include <iostream>

DayCycle::DayCycle() {
	timeStage = 6;
	r = 255; g = 255; b = 255;
	timeCountdown = 60;
	increasing = false;
}

void DayCycle::initialize(sf::RenderTexture* t, sf::Vector2f size) {
	lighting = t;
	lighting->create(size.x, size.y);
}

void DayCycle::update(Player* p, float dt, Vendor* v, std::vector<Room*> rooms, AudioManager* audio) {
	if (timeCountdown > 0) {
		timeCountdown -= dt;
	} else {
		if (increasing) {
			timeStage++;
			if (timeStage == 6) {
				increasing = false;
			}
		} else {
			timeStage--;
			if (timeStage == 0) {
				increasing = true;
			}
		}

		if (timeStage == 0 || timeStage == 6) {
			timeCountdown = 60;
		}
		else {
			timeCountdown = 6;
		}

		switch (timeStage) {
			case 0:
				r = 15; g = 15; b = 15;
				audio->stopAllMusic();
				audio->playMusicbyName("Night");
				break;
			case 1:
				r = 55; g = 55; b = 55;
				break;
			case 2:
				r = 95; g = 95; b = 95;
				break;
			case 3:
				r = 135; g = 135; b = 135;
				break;
			case 4:
				r = 175; g = 175; b = 175;
				break;
			case 5:
				r = 215; g = 215; b = 215;
				break;
			case 6:
				r = 255; g = 255; b = 255;
				v->restock(2);
				for (auto room : rooms) {
					for (int i = 0; i < room->getChestList().size(); i++) {
						room->getChestList()[i]->spawnLoot();
					}
				}
				audio->stopAllMusic();
				audio->playMusicbyName("Day");
				break;
		}
	}

	color = sf::Color(r, g, b, 0);
}

void DayCycle::drawLight(sf::RenderWindow* w, sf::View* v, std::vector<Lantern*> list) {
	//lighting->setView(*v);
	lighting->clear(color);

	bool visibleX;
	bool visibleY;

	for (int i = 0; i < list.size(); i++) {
		visibleX = list[i]->getPosition().x + list[i]->getSize().x/2 > v->getCenter().x - screenX * 0.9 / 2 && list[i]->getPosition().x  - list[i]->getSize().x / 2 < v->getCenter().x + screenX * 0.9 / 2;
		visibleY = list[i]->getPosition().y + list[i]->getSize().y/2 > v->getCenter().y - screenY * 0.9 / 2 && list[i]->getPosition().y  - list[i]->getSize().y / 2 < v->getCenter().y + screenY * 0.9 / 2;

		if (visibleX && visibleY) {
			lighting->draw(*list[i], sf::BlendAdd);
		}
	}

	lighting->display();

	renderLight.setTexture(lighting->getTexture());

	w->draw(renderLight, sf::BlendMultiply);
}

const int DayCycle::getTime() {
	return timeStage;
}