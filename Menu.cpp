#include "Menu.h"

// Default constructor
Menu::Menu() {
	// Load font
	font.loadFromFile("font/Daydream.ttf");

	// Set menu properties
	setFillColor(sf::Color(160, 120, 100, 255));
	setOutlineThickness(screenX / 200);
	setOutlineColor(sf::Color(120, 80, 60, 255));

	// Offset position using outline thickness
	setPosition(getOutlineThickness(), getOutlineThickness());

	// Set size of menu to cover full screen
	setSize({ (float)screenX - getOutlineThickness() * 2, (float)screenY - getOutlineThickness() * 2 });
}

// Overloaded constructor
Menu::Menu(int id) {
	// Load font
	font.loadFromFile("font/Daydream.ttf");

	// Set menu properties
	setFillColor(sf::Color(160, 120, 100, 255));
	setOutlineThickness(screenX / 200);
	setOutlineColor(sf::Color(120, 80, 60, 255));

	// Offset position using outline thickness
	setPosition(getOutlineThickness(), getOutlineThickness());

	// Set size of menu to cover full screen
	setSize({ (float)screenX - getOutlineThickness() * 2, (float)screenY - getOutlineThickness() * 2 });

	// Set contents of menu according to ID
	switch (id) {

	// Main Menu
	case 0:
		// Add start button
		addButton({ 10,20 }, { screenX * 3 / 20.f, screenY * 10 / 12.f }, 0);
		buttons[0]->setDescription("Start");
		// Add controls button (main menu)
		addButton({ 10,20 }, { screenX * 7 / 20.f, screenY * 10 / 12.f }, 3);
		// Add instructions button (main menu)
		addButton({ 10,20 }, { screenX * 11 / 20.f, screenY * 10 / 12.f }, 5);
		// Add exit button
		addButton({ 10,20 }, { screenX * 15 / 20.f, screenY * 10 / 12.f }, 9);

		// Create title text & set properties
		allText.push_back(new sf::Text);
		allText[0]->setCharacterSize(screenX / 10);
		allText[0]->setString(" Abyssal\n  Village");
		allText[0]->setPosition(screenX / 10.f, screenY / 15.f);
		break;

	// Pause Menu
	case 1:
		// Add resume button
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)0.6 * 2.f / 20.f }, 0);
		// Add main menu button
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)1.9 * 2.f / 20.f }, 1);
		// Add controls button (pause menu)
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)3.2 * 2.f / 20.f }, 4);
		// Add instructions button (pause menu)
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)4.5 * 2.f / 20.f }, 6);
		// Add save button
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)5.8 * 2.f / 20.f }, 7);
		// Add load button
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)7.1 * 2.f / 20.f }, 8);
		// Add exit button
		addButton({ 5,20 }, { screenX * 8 / 20.f, screenY * (float)8.4 * 2.f / 20.f }, 9);
		break;

	// Controls menu (returns to main menu)
	case 2:

		// Add main menu button
		addButton({ 10,20 }, { screenX / 30.f, screenY * 13 / 15.f }, 1);

		// Create text for controls
		allText.push_back(new sf::Text);
		allText[0]->setPosition(screenX / 10.f, screenY / 15.f);
		allText[0]->setString("Controls:\n\nMove - WASD\n\nInteract - F\n\nEscape - Pause\n\nNavigate Menus - Mouse\n\nInventory -\n   Arrow keys to move\n   I to open full inventory\n   Q to pick up stack & place in new slot\n   E to pick up single item from stack\n\nShops -\n   Arrow keys to move\n   F to buy and sell\n   Hold shift to buy or sell all\n");
		break;

	// Instructions menu (returns to main menu)
	case 3:

		// Add main menu button
		addButton({ 10,20 }, { screenX / 30.f, screenY * 13 / 15.f }, 1);

		// Create text for instructions
		allText.push_back(new sf::Text);
		allText[0]->setPosition(screenX / 10.f, screenY / 15.f);
		allText[0]->setString("Your goal is to earn 1000 gold by harvesting crops and eggs\n and selling them to the shopkeeper at the wagon.\n\n Your family will get hungry over time, so make sure to give them food!");
		break;

	// Controls menu (returns to pause menu)
	case 4:

		// Add pause menu button
		addButton({ 10,20 }, { screenX / 30.f, screenY * 13 / 15.f }, 2);

		// Create text for controls
		allText.push_back(new sf::Text);
		allText[0]->setPosition(screenX / 10.f, screenY / 15.f);
		allText[0]->setString("Controls:\n\nMove - WASD\n\nInteract - Space\n\nEscape - Pause\n\nNavigate Menus - Mouse\n\nInventory -\n   Arrow keys to move\n   I to open full inventory\n   Q to pick up stack & place in new slot\n   E to pick up single item from stack\n\nShops -\n   Arrow keys to move\n   F to buy and sell\n   hold shift to buy or sell all\n   R to refresh stock");
		break;

	// Instructions menu (returns to pause menu)
	case 5:

		// Add pause menu button
		addButton({ 10,20 }, { screenX / 30.f, screenY * 13 / 15.f }, 2);

		// Create text for instructions
		allText.push_back(new sf::Text);
		allText[0]->setPosition(screenX / 10.f, screenY / 15.f);
		allText[0]->setString("Your goal is to earn 1000 gold by harvesting crops and eggs\n and selling them to the shopkeeper at the wagon.\n\n Your family will get hungry over time, so make sure to give them food!");
		break;

	// Game Over menu
	case 6:

		// Add main menu button
		addButton({ 10,20 }, { screenX * 9.f / 20.f, screenY * 10 / 12.f }, 1);

		// Create game over text
		allText.push_back(new sf::Text);
		allText[0]->setCharacterSize(screenX / 10);
		allText[0]->setString("Game\nOver");
		allText[0]->setPosition(screenX * 9 / 30.f, screenY / 15.f);
		break;

	// Win Menu
	case 7:
		// Add main menu button
		addButton({ 10,20 }, { screenX * 9.f / 20.f, screenY * 10 / 12.f }, 1);

		// Create win text
		allText.push_back(new sf::Text);
		allText[0]->setCharacterSize(screenX / 10);
		allText[0]->setString("You\nWin!");
		allText[0]->setPosition(screenX * 9 / 30.f, screenY / 15.f);
		break;

	}

	// Set font of all created text
	for (auto& text : allText) {
		text->setFont(font);
	}

}

// handleInput function
// Calls input functions for all buttons in menu
void Menu::handleInput(bool* isPaused, int* menu, Save* save, Save* save2, Level* l) {
	for (auto& button : buttons) {
		button->handleInput(isPaused, menu, save, save2, l);
	}
}

// Update function
// Calls update functions for all buttons in menu
void Menu::update(Cursor m) {
	for (auto& button : buttons) {
		button->update(m);
	}
}

// addButton function
// Adds a button to the buttons vector using the size, position, and ID parameters
void Menu::addButton(sf::Vector2f size, sf::Vector2f position, int buttonID) {
	Button* b = new Button(size, position, buttonID);
	buttons.push_back(b);
}

// Draw function
// Draws all elements of menu in specified window
void Menu::draw(sf::RenderWindow* w) {
	w->draw(*this);

	for (auto& button : buttons) {
		button->draw(w);
	}

	for (auto& text : allText) {
		w->draw(*text);
	}
}

// setInput function
// Sets input for all buttons in menu
void Menu::setInput(Input* input) {
	// Call gameObject's setInput to prevent infinite loop
	GameObject::setInput(input);

	for (auto& button : buttons) {
		button->setInput(input);
	}
}

