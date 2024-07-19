#pragma once

#include "Sprite.h"

class Ball : public Sprite {
public:
	float lastStartSpeed;
	Ball(float size, string iname);
	void saveLastStartSpeed();
	virtual void update();
};
