#include "Save.h"

// Constructor
Save::Save() {
	limit = 3;
}

// addSave function
// Adds a new copy of level to saves vector
void Save::addSave(Level l) {
	Level level = l;
	saves.push_back(level);
}

// replaceSave function
// Replaces level object in saves vector at index with new copy of level
void Save::replaceSave(int index, Level l) {
	if (limit > 0) {
		limit--;
		Level level = l;
		saves[index] = level;
	}
}

// resetTo function
// Sets the properties of level parameter to match copy at index in saves vector
void Save::resetTo(int index, Level* l) {
	*l = saves[index];
}