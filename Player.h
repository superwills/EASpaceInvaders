#pragma once

#include "Sprite.h"

class Bullet;

class Player : public Sprite {
public:
  // Sizing is based on world size.
	Player( const Vector2f &windowSize );
  
  shared_ptr<Bullet> shoot(); 
};
