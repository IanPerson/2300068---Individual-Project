#pragma once
#include "Level.h"

// Forward declaration for necessary classes
class Level;

class Save {
	private:
		// Vector of level copies
		std::vector<Level> saves;

		// Limit for saving
		int limit;

	public:
		// Constructor
		Save();

		// addSave function
		// Adds a new copy of level to saves vector
		void addSave(Level l);

		// replaceSave function
		// Replaces level object in saves vector at index with new copy of level
		void replaceSave(int index, Level l);

		// resetTo function
		// Sets the properties of level parameter to match copy at index in saves vector
		void resetTo(int index, Level* l);
};

