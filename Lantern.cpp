#include "Lantern.h"

Lantern::Lantern(sf::Vector2f s, sf::Vector2f pos) {
	size = s;
	setSize(size);
	setPosition(pos);
	setOrigin(s.x / 2, s.y / 2);

	position = pos;

	animations[0].loadFromFile("gfx/Lantern_Lit.png");
	animations[1].loadFromFile("gfx/Lantern_Unlit.png");

	for (int i = 0; i < 5; i++) {
		lanternAnimation.addFrame(sf::IntRect(i*16,0,16,28));
	}

	collisionBox.width = size.x / 2;
	collisionBox.height = size.y / 2;
	collisionBox.left = collisionBox.width / 2;
	collisionBox.top = collisionBox.height / 2;
	setCollisionBox(collisionBox);

	isActive = true;
	used = false;

	lightCountdown = 20;

	lanternAnimation.setFrameSpeed(1 / 6.f);

	lanternSprite.setTexture(&animations[0]);
	lanternSprite.setTextureRect(lanternAnimation.getCurrentFrame());

	lanternSprite.setSize({ 32, 56 });
	lanternSprite.setPosition(pos.x - lanternSprite.getSize().x / 2, pos.y - lanternSprite.getSize().y / 2);
}

void Lantern::update(Player* p, float dt) {

	lanternAnimation.animate(dt);
	lanternSprite.setTextureRect(lanternAnimation.getCurrentFrame());

	if (p->getCollisionBox().intersects(getCollisionBox())) {
		keyboardInteract = true;
		if (interact == 1) {
			if (!isActive) {
				p->setPrompt("F to relight");
			} else {
				p->setPrompt("F to pick up");
			}
			p->setInteract(p->getInteract() + 1);
			interact -= 1;
		}
	} else {
		keyboardInteract = false;
		if (interact == 0) {
			p->setPrompt("");
			p->setInteract(p->getInteract() - 1);
			interact += 1;
		}
	}

	if (lightCountdown > 0) {
		lightCountdown -= (rand() % 4) * dt;
	} else if (lanternSprite.getTexture() != &animations[1] && isActive) {
		lanternSprite.setTexture(&animations[1]);
		setFillColor(sf::Color(getFillColor().r, getFillColor().g, getFillColor().b, getFillColor().a / 4));
		isActive = false;
	}

}

void Lantern::handleInput(Player* p, Input* i) {
	if (i->isPressed(sf::Keyboard::F) && !isActive && keyboardInteract) {
		lanternSprite.setTexture(&animations[0]);
		isActive = true;
		lightCountdown = 20;
		setFillColor(sf::Color(getFillColor().r, getFillColor().g, getFillColor().b, getFillColor().a * 4));

	} else if (i->isPressed(sf::Keyboard::F) && isActive && keyboardInteract) {
		p->setInteract(p->getInteract() - 1);
		p->carryLantern(true);
		setUsed(true);
	}
}

void Lantern::drawLantern(sf::RenderWindow* w) {
	w->draw(lanternSprite);
}

void Lantern::setActive(bool b) {
	isActive = b;
}

void Lantern::setUsed(bool b) {
	used = b;
}

bool Lantern::getUsed() {
	return used;
}

bool Lantern::getActive() {
	return isActive;
}

void Lantern::reset() {
	setSize(size);
	setFillColor(sf::Color(230, 230, 180, 100));
}

sf::Vector2f Lantern::getOriginalSize() {
	return size;
}