#pragma once

#include "Sprite.h"

class Player : public Sprite {
	float speed;

public:
	Player( Vector2f size, float iSpeed );
	void moveDown();
	void moveUp();
};
