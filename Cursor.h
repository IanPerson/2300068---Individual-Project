#pragma once
#include "Framework/GameObject.h"

class Cursor : public GameObject {
	private:
		// Variables
		sf::Texture texture;

	public:
		// Constructor and deconstructor
		Cursor();
		~Cursor();
		
		// Update function
		// Input i -> get position of mouse on screen
		void update(Input i);
};

