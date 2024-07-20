#pragma once

#include "Sprite.h"
#include "Vectorf.h"

class Bullet : public Sprite {
public:
  inline static vector< Bullet* > allBullets;
  static void clearDead();
  
	Bullet( const RectF &playerBox );
  ~Bullet(); 
	void update() override;
};
