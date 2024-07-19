#pragma once

#include "Sprite.h"

class Ball : public Sprite {
public:
	float lastStartSpeed;
	Ball( float size );
	void saveLastStartSpeed();
	virtual void update();
};
