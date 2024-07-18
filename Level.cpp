#include "Level.h"
#include <windows.h>
using namespace std;

Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud) {
	srand(time(0));
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;

	// initialise game objects

	// Set size & position of window according to size of computer screen
	window->setSize(sf::Vector2u(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)));

	// Limit framerate to 60
	window->setFramerateLimit(60);

	// Disable repeating keys
	window->setKeyRepeatEnabled(true);

	audio->addMusic("sfx/Dark Forest.wav", "Night");
	audio->addMusic("sfx/Village.wav", "Day");
	audio->addSound("sfx/Ambience 1.wav", "Ambience1");
	audio->addSound("sfx/Ambience 2.wav", "Ambience2");
	audio->addSound("sfx/Growl 1.wav", "Growl1");
	audio->addSound("sfx/Growl 2.wav", "Growl2");

	audio->playMusicbyName("Day");

	// Disable mouse visibility
	window->setMouseCursorVisible(false);

	// Default states
	isPaused = true;
	currentlyTrading = false;

	// Default position in menu & room vectors
	currentMenu = 0;
	currentRoom = 0;

	// Load font
	font.loadFromFile("font/arial.ttf");

	// Load frame rate text properties
	frameRate.setFont(font);
	frameRate.setFillColor(sf::Color::White);
	frameRate.setCharacterSize(50);

	// Initialize inventory
	inventory.setWindow(window);
	inventory.setInput(input);
	inventory.initialize();

	// Set initial player position
	player.setPosition({ 1750,1800 });

	// Create animals and push into vector
	for (int i = 0; i < 15; i++) {
		Animal a({1100,1000}, {8000,8000});
		a.setPosition(rand() % 8000 + 1100, rand() % 8000 + 1000);
		animals.push_back(a);
	}

	// Add seeds to inventory
	for (int i = 0; i < 2; i++) {
		inventory.addToInventory(i, 5);
	}

	// Set outside tile maps
	farm->addTileMap(0, "Maps/Map1_Grass.csv");
	farm->addTileMap(3, "Maps/Map1_Path.csv");
	farm->addTileMap(1, "Maps/Map1_Hills.csv", "Maps/Map1_Collision.csv");
	farm->addLantern({ 1550,1600 });
	farm->addLantern({ 2850,3600 });
	farm->addLantern({ 2850,5000 });
	farm->addLantern({ 4850,3600 });
	farm->addLantern({ 5750,3600 });
	farm->addLantern({ 4850,6000 });
	farm->addLantern({ 4950,7500 });

	// Add objects to rooms & fields vector
	rooms.push_back(farm);
	rooms.push_back(home.getRoom());
	rooms.push_back(house1.getRoom());
	rooms.push_back(house2.getRoom());
	rooms.push_back(house3.getRoom());
	rooms.push_back(house4.getRoom());
	rooms.push_back(house5.getRoom());
	rooms.push_back(house6.getRoom());

	houses.push_back(&home);
	houses.push_back(&house1);
	houses.push_back(&house2);
	houses.push_back(&house3);
	houses.push_back(&house4);
	houses.push_back(&house5);
	houses.push_back(&house6);

	for (int i = 1; i < rooms.size(); i++) {
		for (int j = 0; j < rand() % 4; j++) {
			rooms[i]->addChest(rand() % 3 + 1, { rand() % 1000 + 1100.f, rand() % 900 + 1000.f });
			Sleep(0.1);
		}
	}

	// Push all fields to vector
	fields.push_back(field1);
	fields.push_back(field2);
	fields.push_back(field3);
	fields.push_back(field4);
	fields.push_back(field5);

	// Set camera properties
	camera = window->getView();
	camera.zoom(0.8);

	// Set ui properties
	ui = window->getView();
	ui.zoom(1);
	ui.setSize(sf::Vector2f(window->getSize()));
	ui.setCenter({ window->getSize().x / 2.f, window->getSize().y / 2.f });
	
	// Set inputs
	mouse.setInput(input);
	player.setInput(input);
	family.setInput(input);

	// Set shopkeeper input and window
	shopkeeper.setInput(input);
	shopkeeper.setWindow(window);

	spawner.setInput(input);

	// Create menus and push to vector
	for (int i = 0; i < 8; i++) {
		menus.push_back(new Menu(i));
		menus[i]->setInput(input);
	}

	cycle.initialize(new sf::RenderTexture, rooms[currentRoom]->getSize());


	// Create two save objects
	saves.push_back(new Save);
	saves.push_back(new Save);

	// Add save to both save objects
	saves[0]->addSave(*this);
	saves[1]->addSave(*this);

}

Level::~Level() {

}

// handle user input
void Level::handleInput(float dt) {

	// If escape is pressed, pause game
	if (input->isPressed(sf::Keyboard::Escape) && !isPaused) {
		isPaused = true;
		currentMenu = 1;
	}
	// If escape is pressed while paused and the player is currently in the pause menu, unpause game
	else if (input->isPressed(sf::Keyboard::Escape) && isPaused && currentMenu == 1) {
		isPaused = false;
	}

	// If i is pressed, enter/exit inventory
	if (input->isPressed(sf::Keyboard::I) && !inInventory) {
		inInventory = true;
	}
	else if (input->isPressed(sf::Keyboard::I) && inInventory) {
		inInventory = false;
	}

	// If the game isn't paused, call input functions
	if (!isPaused && !currentlyTrading) {
		player.handleInput(dt);
		rooms[currentRoom]->handleInput(input, &currentRoom, &player, &inventory);
		// Call field and animal input functions if outside
		if (currentRoom == 0) {
			for (int i = 0; i < fields.size(); i++) {
				fields[i].handleInput(player, input, camera, &inventory);
			}

			for (int i = 0; i < animals.size(); i++) {
				animals[i].handleInput(input, &player, &inventory);
			}

			for (auto home : houses) {
				home->handleInput(input, &currentRoom, &player);
			}

			spawner.handleInput(&player, input, currentRoom);
		}
	}
	
	// If paused, call input function for current menu
	// Otherwise, call shopkeeper's input function if outside
	if (isPaused) {
		menus[currentMenu]->handleInput(&isPaused, &currentMenu, saves[0], saves[1], this);
	} else if (currentRoom == 0 && cycle.getTime() >= 4) {
		shopkeeper.handleInput(&inventory);
	}
	
	// If not trading, call input function for inventory
	if (!currentlyTrading) {
		inventory.handleInput(inInventory);
	}
}

// Update game objects
void Level::update(float dt) {
	// Update trade state based on shopkeeper's state
	if (shopkeeper.tradeState()) {
		currentlyTrading = true;
	} else {
		currentlyTrading = false;
	}

	// Update inventory
	inventory.update(inInventory, &currentMenu, &isPaused);

	spawner.update(dt, cycle.getTime(), &player, currentRoom, audio, rooms[0]->getLanterns());

	// Update cursor and buttons
	mouse.update(*input);

	// Update size and center of ui according to the size of the window
	ui.setCenter({ window->getSize().x / 2.f, window->getSize().y / 2.f });
	ui.setSize(sf::Vector2f(window->getSize()));

	// Update current room
	rooms[currentRoom]->update(&player, camera, dt);

	//frameRate.setString(to_string((int)(1/dt)));
	frameRate.setString(to_string(player.getInteract()));

	// If the game isn't paused, call update functions for appropriate objects
	if (!isPaused) {

		// Stop player while trading, update otherwise
		if (shopkeeper.tradeState()) {
			player.stop();
		} else {
			player.update(dt, &isPaused, &currentMenu);
		}
		
		// Update fields
		for (int i = 0; i < fields.size(); i++) {
			fields[i].update(&player, camera, dt);
		}

		for (auto& chicken : animals) {
			chicken.countdown(dt);
		}

		// If outside, update outside objects
		if (currentRoom == 0) {
			// Update animals
			for (int i = 0; i < animals.size(); i++) {
				animals[i].update(&player, dt);
			}

			// Update shopkeeper & check collision
			if (cycle.getTime() >= 4) {
				shopkeeper.update(&player);
				shopkeeper.collisionCheck(&player, camera, dt);
			}

			for (auto home : houses) {
				home->update(&player);
			}
		}

		// Update family
		if (currentRoom == 1) {
			family.update(&player, dt, &inventory, &currentMenu, &isPaused);
		}

		// Have camera follow player
		camera.setCenter(player.getPosition().x + player.getSize().x / 2, player.getPosition().y + player.getSize().y / 2);

	} else {
		// Update current menu
		menus[currentMenu]->update(mouse);
	}

	cycle.update(&player, dt, &shopkeeper, rooms, audio);

	
}

// Render level
void Level::render() {
	beginDraw();

	// Draw environment
	rooms[currentRoom]->draw(window, camera);

	// If outside, draw outside entities
	if (currentRoom == 0) {

		// Draw fields
		for (int i = 0; i < fields.size(); i++) {
			fields[i].drawCrops(window->getView(), window);
		}

		for (auto& home : houses) {
			window->draw(*home);
		}

		rooms[0]->drawLanterns(window);
		
		// Draw animals
		for (auto& animal : animals) {
			window->draw(animal);
		}

		spawner.drawEnemies(window);

		// Draw player & shopkeeper in a different order depending on player's y position
		if (cycle.getTime() >= 4) {
			if (player.getPosition().y < shopkeeper.getPosition().y + shopkeeper.getSize().y / 3) {
				player.draw(window);
				window->draw(shopkeeper);
			}
			else {
				window->draw(shopkeeper);
				player.draw(window);
			}
		} else {
			player.draw(window);
		}
		

	// Otherwise draw inside entities
	} else {
		player.draw(window);
		if (currentRoom == 1) {
			window->draw(family);
			window->draw(family.getText());
		}
	}

	cycle.drawLight(window, &camera, rooms[currentRoom]->getLanterns());

	// Set view to ui
	window->setView(ui);

	// Draw vendor stock if trading
	if (shopkeeper.tradeState() && !isPaused) {
		shopkeeper.drawStock(window);

	// Otherwise, draw inventory
	} else {
		if (inInventory) {
			window->draw(inventory);
			inventory.drawInv();
		}
		inventory.drawHotbar();
	}

	// Draw menus if paused
	if (isPaused) {
		menus[currentMenu]->draw(window);
	}

	// Draw cursor
	window->draw(mouse);

	//window->draw(frameRate);

	// Reset view to camera
	window->setView(camera);

	endDraw();
}