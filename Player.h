#pragma once

#include "Sprite.h"

class Player : public Sprite {
public:
  // Sizing is based on world size.
	Player( const Vector2f &worldSize );
  
  // Player restricted to moving in x.
	void move( float x );
};
