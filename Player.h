#pragma once

#include "Sprite.h"

class Bullet;

class Player : public Sprite {
public:
  inline static const float PlayerSpeed = 350;
  Player( const RectF &rectangle );
  void update( float t ) override;
  void die() override;
  
  void setMovingLeft();
  void setMovingRight();
  
};
