#pragma once

#include "Entity.h"

class Tile : public Entity {
public:
	Tile(int, int, int, int);
	~Tile();

	virtual void update(float);

	virtual void hit(Entity*);

	bool isSolid();

protected:
	bool solid = false;
};

