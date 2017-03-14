#include "Render.h"


Render::Render(Model* newModel){
	model = newModel;

	windowWidth = 800;
	windowHeight = 600;

	window.create(sf::VideoMode(windowWidth, windowHeight), "LIVE DEMO");

	window.setFramerateLimit(60);

	camera.setSize(sf::Vector2f(windowWidth, windowHeight));

	menuCamera.setSize(sf::Vector2f(410, 310));
	menuCamera.setCenter(-1202.5, 155.0);

	font.loadFromFile("Assets/times.ttf");
	
	textBrush.setFont(font);
	textBrush.setCharacterSize(20);
	textBrush.setFillColor(sf::Color::White);
	textBrush.setStyle(sf::Text::Regular);

	modelTexture.create(windowWidth, windowHeight);
	invMenuTexture.create(windowWidth, windowHeight);
	craftMenuTexture.create(windowWidth, windowHeight);
}

Render::~Render(){ }

void Render::render() {
	window.clear();

	renderModel();
	sf::Sprite modelSprite;
	modelSprite.setTexture(modelTexture.getTexture());
	window.draw(modelSprite);

	if (model->gameMode == 1) {
		renderCraftMenu();
		sf::Sprite craftMenuSprite;
		craftMenuSprite.setTexture(craftMenuTexture.getTexture());
		window.draw(craftMenuSprite);
	}
	else if (model->gameMode == 2) {
		renderInvMenu();
		sf::Sprite invMenuSprite;
		invMenuSprite.setTexture(invMenuTexture.getTexture());
		window.draw(invMenuSprite);
	}

	window.display();
}

void Render::renderModel() {
	//finding camera position
	camPosition = model->player->getPosition();
	if (camPosition.x < windowWidth / 2)
		camPosition.x = windowWidth / 2;
	else if (camPosition.x > model->mapWidth * model->tileSize - windowWidth / 2)
		camPosition.x = model->mapWidth * model->tileSize - windowWidth / 2;

	if (camPosition.y < windowHeight / 2)
		camPosition.y = windowHeight / 2;
	else if (camPosition.y > model->mapHeight * model->tileSize - windowHeight / 2)
		camPosition.y = model->mapHeight * model->tileSize - windowHeight / 2;
	camera.setCenter(camPosition);

	modelTexture.clear();
	//updating the camera position
	modelTexture.setView(camera);

	//rendering tilemap
	for (int y = 0; y < model->mapHeight; y++) {
		for (int x = 0; x < model->mapWidth; x++) {
			modelTexture.draw(model->tileMap[y][x]->sprite);
		}
	}

	//rendering player
	model->player->sprite.setPosition(model->player->getPosition());
	modelTexture.draw(model->player->sprite);

	for (std::vector<Enemy*>::iterator i = model->enemies.begin(); i != model->enemies.end(); i++) {
		(*i)->sprite.setPosition((*i)->getPosition());
		modelTexture.draw((*i)->sprite);
	}

	//rendering attacks
	for (std::vector<Attack*>::iterator i = model->attacks.begin(); i != model->attacks.end(); i++) {
		(*i)->sprite.setPosition((*i)->getPosition());
		modelTexture.draw((*i)->sprite);
	}

	//rendering the area of affect for sounds
	for (std::vector<Sound>::iterator i = model->sounds.begin(); i != model->sounds.end(); i++) {
		sf::CircleShape soundSphere = sf::CircleShape(i->getLoudness());
		soundSphere.setPosition(i->getPosition() - sf::Vector2f(soundSphere.getRadius(), soundSphere.getRadius()));
		soundSphere.setFillColor(sf::Color(0, 0, 255, 100));
		modelTexture.draw(soundSphere);
	}

	//rendering loot
	for (std::vector<Loot*>::iterator i = model->droppedLoot.begin(); i != model->droppedLoot.end(); i++)
		modelTexture.draw((*i)->sprite);

	modelTexture.display();
}

void Render::renderCraftMenu() {
	craftMenuTexture.clear(sf::Color::Transparent);

	border = sf::RectangleShape(sf::Vector2f(410, 310));
	border.setPosition(sf::Vector2f(195, 145));
	border.setFillColor(sf::Color::White);
	craftMenuTexture.draw(border);

	center = sf::RectangleShape(sf::Vector2f(400, 300));
	center.setPosition(sf::Vector2f(200, 150));
	center.setFillColor(sf::Color(100, 100, 100));
	craftMenuTexture.draw(center);

	divide = sf::RectangleShape(sf::Vector2f(5, 300));
	divide.setPosition(sf::Vector2f(330, 150));
	divide.setFillColor(sf::Color::White);
	craftMenuTexture.draw(divide);

	textBrush.setString("Items:");
	textBrush.setPosition(sf::Vector2f(205, 150));
	craftMenuTexture.draw(textBrush);

	int counter = 0;
	for (std::map<std::string, Item*>::iterator i = model->craftMenu->itemList.begin(); i != model->craftMenu->itemList.end(); i++) {
		//std::cout << model->itemManager->itemIndex.at(i+1) << std::endl;
		(*i).second->menuIcon.setPosition(sf::Vector2f(200, 175 + 75 * counter));
		craftMenuTexture.draw((*i).second->menuIcon);

		if (!model->craftMenu->canMake[counter]) {
			sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(130, 70));
			rect.setPosition(sf::Vector2f(200, 175 + 75 * counter));
			rect.setFillColor(sf::Color(0, 0, 0, 100));
			craftMenuTexture.draw(rect);
		}
		//textBrush.setPosition(sf::Vector2f(-1395, 30 * (i + 1)));
		//window.draw(textBrush);
		counter++;
	}
	model->craftMenu->selectedBox.setPosition(200, 175 + 75 * model->craftMenu->curSelected);
	craftMenuTexture.draw(model->craftMenu->selectedBox);

	model->craftMenu->itemList.at(model->craftMenu->itemNameList.at(model->craftMenu->curSelected))->menuDescription.setPosition(sf::Vector2f(335, 150));
	craftMenuTexture.draw(model->craftMenu->itemList.at(model->craftMenu->itemNameList.at(model->craftMenu->curSelected))->menuDescription);

	craftMenuTexture.display();
}

void Render::renderInvMenu() {
	invMenuTexture.clear(sf::Color::Transparent);

	border = sf::RectangleShape(sf::Vector2f(410, 310));
	border.setPosition(sf::Vector2f(195, 145));
	border.setFillColor(sf::Color::Black);
	invMenuTexture.draw(border);

	center = sf::RectangleShape(sf::Vector2f(400, 300));
	center.setPosition(sf::Vector2f(200, 150));
	center.setFillColor(sf::Color::White);
	invMenuTexture.draw(center);

	model->invMenu->iMenuSprite.setPosition(200, 150);
	invMenuTexture.draw(model->invMenu->iMenuSprite);

	int counter = 0;
	Inventory* inv = model->player->getInventory();
	for (int y = 0; y < inv->getHeight(); y++)
		for (int x = 0; x < inv->getWidth(); x++) {
			Item* curItem = inv->getCurSeletected(x, y);
			if (curItem != NULL) {
				curItem->smallIcon.setPosition(
					200 + model->invMenu->iSlotStart.x + x * (model->invMenu->iSlotOff.x + model->invMenu->iSlotDim.x),
					150 + model->invMenu->iSlotStart.y + y * (model->invMenu->iSlotOff.y + model->invMenu->iSlotDim.y));
				curItem->smallIcon.setScale(1.0 / (curItem->smallIcon.getLocalBounds().width / model->invMenu->iSlotDim.x),
					1.0 / (curItem->smallIcon.getLocalBounds().height / model->invMenu->iSlotDim.y));
				invMenuTexture.draw(curItem->smallIcon);
			}
		}

	model->invMenu->selectedBox.setPosition(
		200 + model->invMenu->iSlotStart.x + ((model->invMenu->iSlotDim.x + model->invMenu->iSlotOff.x) * model->invMenu->curX),
		150 + (model->invMenu->iSlotStart.y + ((model->invMenu->iSlotDim.y + model->invMenu->iSlotOff.y) * model->invMenu->curY)));
	model->invMenu->selectedBox.setOutlineColor(sf::Color::Green);
	invMenuTexture.draw(model->invMenu->selectedBox);

	invMenuTexture.display();
}
