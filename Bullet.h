#pragma once

#include "Sprite.h"
#include "Vectorf.h"

class Bullet : public Sprite {
public:
  inline static vector< Bullet* > allBullets;
  
	Bullet( const RectF &playerBox );
  ~Bullet(); 
	virtual void update();
};
