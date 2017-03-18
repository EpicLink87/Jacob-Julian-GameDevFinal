#pragma once

#include "Model.h"
#include "AssetManager.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <sstream>

class Render {
public:
	sf::RenderWindow window;
	int windowWidth, windowHeight;

	Render(Model*);
	~Render();

	void initializeRenderable(Renderable*);

	void drawRenderable(sf::RenderTarget&, Renderable*);

	void render();

	void renderModel();
	void renderUI();
	void renderInvMenu();
	void renderCraftMenu();
	void renderMainMenu();
	void renderWin();
	void renderLose();

	void renderPlayerVertexArray();
	void renderPlayerSpriteArray();
	AssetManager manager;

	sf::View camera;
	sf::Vector2f camPosition;

	sf::RenderTexture modelTexture;
	sf::RenderTexture invMenuTexture;
	sf::RenderTexture craftMenuTexture;
	sf::RenderTexture UITexture;
	sf::RenderTexture mainMenuTexture;

	sf::Text textBrush;
	sf::Font font;

	sf::RectangleShape border;
	sf::RectangleShape center;
	sf::RectangleShape divide;

	sf::Texture playerSpriteSheet;
	sf::VertexArray playerSprites;

	/*(std::string input, int x, int y, int textureWidth, int textureHeight, int fontSize, sf::RenderTarget wherToRenderTo*/
	void makeStringTextrue(std::string, int, int, int, int, int, sf::RenderTarget&);

	/* Four arrays:
	   [0]: Size 3, idle animation.
	   [1]: Size 6, running animation.
	   [2]: Size 3, jump animation, [2][1] and [2][2] are landing animations.
	   [3]: Size 2, climbing animation.
	*/
	sf::Sprite** playerSpriteArray;

	bool menuOpen = false;

	float screenAlpha = 0;
private:
	Model* model;

};

