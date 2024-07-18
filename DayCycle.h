#pragma once
#include "Framework/GameObject.h"
#include "Lantern.h"
#include "Vendor.h"
#include "Room.h"
#include "Framework/AudioManager.h"

class DayCycle : public GameObject {
	private:
		// Screen dimensions
		int screenX = sf::VideoMode::getDesktopMode().width;
		int screenY = sf::VideoMode::getDesktopMode().height;

		// Variables for lighting & rendering
		std::vector<Lantern*> lanterns;
		sf::RenderTexture* lighting = NULL;
		sf::Sprite renderLight;

		// Color variables
		float r; float g; float b;
		sf::Color color;

		// Time progression
		bool increasing;
		int timeStage;
		float timeCountdown;


	public:
		// Constructor
		DayCycle();

		// Initializes lighting render texture
		void initialize(sf::RenderTexture* t, sf::Vector2f size);

		// Progresses time and updates the environment
		void update(Player* p, float dt, Vendor* v, std::vector<Room*> r, AudioManager* audio);

		// Renders the lighting for the current time
		void drawLight(sf::RenderWindow* w, sf::View* v, std::vector<Lantern*> list);

		// Returns the current stage of time
		const int getTime();

};

