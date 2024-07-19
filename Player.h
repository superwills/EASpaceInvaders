#pragma once

#include "Sprite.h"

class Player : public Sprite {
	float speed;

public:
	Player( Vector2f size, float iSpeed );
  
  // Player restricted to moving in x.
	void move( float x );
};
