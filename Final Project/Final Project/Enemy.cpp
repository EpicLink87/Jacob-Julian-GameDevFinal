#include "Enemy.h"



Enemy::Enemy() : Actor(400, 300, 20, 20) {

}

Enemy::Enemy(sf::Vector2f startingPosition, ItemManager* itemManager, MovementMap* map) : Actor(startingPosition.x, startingPosition.y, 20, 20) {
	homePosition = startingPosition + sf::Vector2f(0, 16);
	velocity.x = 70;
	moving = true;
	behaviorState = 1;
	maxHealth = curHealth = 10;

	pathFinder = map;

	textureName = "Actors/Enemy";

	//addInventory(itemManager->getItem("Wood", 1));
	//addInventory(itemManager->getItem("Blade", 1));
	//addInventory(itemManager->getItem("Herb"));

	//patrolPath.push_back(sf::Vector2f(homePosition.x + 64, homePosition.y));
	//patrolPath.push_back(sf::Vector2f(homePosition.x - 64, homePosition.y));

	//targetLocation = patrolPath.at(0);

	//std::cout << patrolPath.size() << std::endl;
}


Enemy::~Enemy() {

}



void Enemy::update(float deltaTime) {

	if (curHealth <= 0) {
		remove();
		return;
	}

	//std::cout << patrolPath.at(curPoint).x << "," << patrolPath.at(curPoint).y << ":" << curPoint << std::endl;

	if (behaviorState == 1) {
		if (patrolPath.size() > 0) {
			if (pow(patrolPath.at(curPoint).x - (position.x + 10), 2) + pow(patrolPath.at(curPoint).y - (position.y + 10), 2) < pow(16, 2)) {
				curPoint++;
				curPoint = (curPoint + patrolPath.size()) % patrolPath.size();
			}
			targetLocation = patrolPath.at(curPoint);
		}
		else {
			targetLocation = homePosition;
		}
		//std::cout << "patrol" << std::endl;
	}
	else if (behaviorState == 2) {

		//if (pow(targetLocation.x - position.x, 2) + pow(targetLocation.y - position.y, 2) < pow(32, 2)) {
		if (pow(targetLocation.x - position.x + 10, 2) + pow(targetLocation.y - position.y + 10, 2) < pow(32, 2)){
			behaviorState = 4;
		}
		//std::cout << "investigate" << std::endl;
	}
	else if (behaviorState == 3) {

		if (pow(targetLocation.x - position.x, 2) + pow(targetLocation.y - position.y, 2) < pow(32, 2)) {
			//making either a melee attack or projectile attack
			if (attackTimer <= 0) {
				attackTimer = 0.5;

				if (targetLocation.x > this->position.x)
					facingRight = true;
				else
					facingRight = false;

				Item* weapon = inventory.getCurSeletected(curItemSelected);
				if (weapon == NULL)
					//newAttacks.push_back(new MeleeAttack(position.x, position.y, 10, 10, dirLooking, this));
					newAttacks.push_back(new MeleeAttack(position.x, position.y, 10, 10, dirLooking, this, weapon));
				else if (weapon->attackType == 0)
					//newAttacks.push_back(new MeleeAttack(position.x, position.y, 10, 10, dirLooking, this));
					newAttacks.push_back(new MeleeAttack(position.x, position.y, 10, 10, dirLooking, this, weapon));
				else
					//newAttacks.push_back(new Projectile(position.x, position.y, 10, 10, sf::Vector2f(200 * cos(dirLooking), 200 * sin(dirLooking)), this));
					newAttacks.push_back(new Projectile(position.x, position.y, 10, 10, sf::Vector2f(200 * cos(dirLooking), 200 * sin(dirLooking)), this, weapon));
			}
			moving = false;
		}
		else moving = true;

		//if (pow(targetLocation.x - position.x, 2) + pow(targetLocation.y - position.y, 2) > pow(400, 2)) {
		/*if (abs(targetLocation.x - position.x) < 32) {
			behaviorState = 4;
		}*/
		//std::cout << "chase" << std::endl;
	}
	else if (behaviorState == 4) {
		targetLocation = homePosition;

		if (pow(homePosition.x - position.x, 2) + pow(homePosition.y - position.y, 2) < pow(64, 2)) {
			behaviorState = 1;
		}
		moving = true;
		//std::cout << "return" << std::endl;
	}



	if (path.size() > 0 && pow(path.back().x - (position.y + 10), 2) + pow(path.back().y - (position.y + 10), 2) < pow(16, 2))
		path.pop_back();

	if (path.size() == 0 || prevTargetLocation.x != targetLocation.x || targetLocation.y != prevTargetLocation.y) //{
	//	path.clear();
	//	path.push_back(targetLocation);
	//}
		path = *(pathFinder->getPath(position + sf::Vector2f(10, 10), targetLocation));
	prevTargetLocation = targetLocation;

	timer -= deltaTime;
	if (timer <= 0) {
		timer = 2;
		std::cout << "Path: "<< path.back().x << "," << path.back().y << "," << path.size() << std::endl;
		std::cout << "Pos: " << position.x << "," << position.y << std::endl;
		std::cout << "target: " << targetLocation.x << "," << targetLocation.y << std::endl;
		std::cout << "behaviour: " << behaviorState << "," << patrolPath.size() << std::endl;
		std::cout << "Looking: " << dirLooking << std::endl;
	}
	//std::cout << behaviorState << std::endl;

	if (path.size() > 0) {
		if (path.back().x < position.x)
			velocity.x = -70;
		else if (path.back().x > position.x + hitBox.getSize().x)
			velocity.x = 70;
		else velocity.x = 0;

		climbing = false;
		droppedDown = false;
		jump = false;
		if (path.back().y < position.y) {
			if (nextToClimbable) {
				climbing = true;
				velocity.y = -70;
			}
			else {
				jump = true;
			}
		}
		else if (path.back().y > position.y + hitBox.getSize().y) {
			droppedDown = true;
			if (nextToClimbable) {
				climbing = true;
				velocity.y = 70;
			}
		}
	}

	if (behaviorState == 3) {
		dirLooking = atan((targetLocation.y - position.y) / (targetLocation.x - position.x));
		if (targetLocation.x - position.x < 0)
			dirLooking += 2 * acos(0);
	}
	else {
		if (velocity.x < 0 && velocity.y > 0)
			dirLooking = 1.5 * acos(0);
		else if (velocity.x > 0 && velocity.y > 0)
			dirLooking = 0.5 * acos(0);
		else if (velocity.x < 0 && velocity.y < 0)
			dirLooking = 2.5 * acos(0);
		else if (velocity.x > 0 && velocity.y < 0)
			dirLooking = 3.5 * acos(0);
		else if (velocity.x < 0)
			dirLooking = 2 * acos(0);
		else if (velocity.x > 0)
			dirLooking = 0;
		else if (velocity.y < 0)
			dirLooking = 3 * acos(0);
		else if (velocity.y > 0)
			dirLooking = acos(0);
	}
	

	if (jump && onGround) {
		velocity.y = -150;
		climbing = false;
		onGround = false;
	}

	if (moving)
		position.x += velocity.x * deltaTime;
	if (moving || !climbing) {
		position.y += velocity.y * deltaTime;
	}

	if (!climbing) {
		velocity.y += 200 * deltaTime;
		if (velocity.y > 200)
			velocity.y = 200;
	}

	attackTimer -= deltaTime;
	/*timer++;
	if (timer % 5 == 0)
		std::cout << position.x << "," << position.y << "," << velocity.x << "," << velocity.y << std::endl;*/
	hitBox.setPosition(position);

	nextToClimbable = false;
	onGround = false;
}

void Enemy::doesHear(Sound* checkSound) {
	sf::Vector2f soundLocation = checkSound->getPosition();
	if (behaviorState != 3 && pow(checkSound->getLoudness(), 2) > pow(position.x - soundLocation.x, 2) + pow(position.y - soundLocation.y, 2)) {
		targetLocation = soundLocation;
		behaviorState = 2;
	}
}

void Enemy::doesSee(Actor* checkActor) {
	sf::Vector2f actorLocation = checkActor->getPosition();

	//checking if the actor is within the enemy's range of sight
	if (canSeePoint(actorLocation)) {
		behaviorState = 3;
		targetLocation = actorLocation;
	}
	else if (behaviorState == 3) {
		if (pow(200, 2) < pow(position.x - actorLocation.x, 2) + pow(position.y - actorLocation.y, 2))
			behaviorState = 2;
		else targetLocation = actorLocation;
	}
}

bool Enemy::canSeePoint(sf::Vector2f point) {
	if (pow(200, 2) > pow(position.x - point.x, 2) + pow(position.y - point.y, 2)) {
		float dirToPoint = atan((point.y - position.y) / (point.x - position.x));
		if (point.x - position.x < 0)
			dirToPoint += 2 * acos(0);

		if (abs(dirToPoint - dirLooking) < acos(0) / 2) {
			return true;
		}
	}
	return false;
}

void Enemy::hitWall(sf::Vector2f newPosition, int dir) {
	setPostion(newPosition);

	if (dir == 0 || dir == 2)
		velocity.y = 0;
	else if (dir == 1 || dir == 3)
		velocity.x = 0;

	if (dir == 2) {
		onGround = true;
	}
}

void Enemy::overEdge() {
	velocity.x = -velocity.x;
}

Loot* Enemy::lootDrop() { 
	Item* droppedItem = inventory.dropRandom();
	//std::cout << droppedItem << std::endl;
	if (droppedItem == NULL)
		return NULL;
	return new Loot(position.x, position.y, 10, 10, droppedItem);
}

void Enemy::setPatrolPath(std::vector<sf::Vector2f>* newPath) { patrolPath = *newPath; }

//items = { new Item("name", HashMap<Integer, Integer>{2:1, 3:2}),
