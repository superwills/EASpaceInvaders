#pragma once

#include "Sprite.h"

class Paddle : public Sprite {
	float speed;

public:
	Paddle( Vector2f size, float iSpeed );
	void moveDown();
	void moveUp();
};
