#include "Enemy.h"
#include "Framework/Collision.h"
#include "Framework/VectorHelper.h"

Enemy::Enemy() {
	// Default speed, set aggro to false
	speed.x = 300;
	speed.y = 300;
	hasAggro = false;
}

void Enemy::detectPlayer(Player* p, float dt) {
	// Center detection circles around enemy
	innerDetectionCircle.setPosition(getPosition().x + getSize().x/2 - innerDetectionCircle.getSize().x / 2, getPosition().y + getSize().y / 2 - innerDetectionCircle.getSize().y / 2);
	outerDetectionCircle.setPosition(getPosition().x + getSize().x/2 - outerDetectionCircle.getSize().x / 2, getPosition().y + getSize().y / 2 - outerDetectionCircle.getSize().y / 2);

	// Track distance between enemy & player
	distanceFrom.x = p->getPosition().x + p->getSize().x/2 - getPosition().x - getSize().x/2;
	distanceFrom.y = p->getPosition().y + p->getSize().y/2 - getPosition().y - getSize().y/2;

	// If player comes within inner detection, set aggro to true
	if (Collision::checkBoundingCircle(p, &innerDetectionCircle)) {
		hasAggro = true;
	}

	// If player leaves the outer detection, set aggro to false
	if (!Collision::checkBoundingCircle(p, &outerDetectionCircle)) {
		hasAggro = false;
	}
}