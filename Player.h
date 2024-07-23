#pragma once

#include "Sprite.h"

class Bullet;

class Player : public Sprite {
public:
  inline static const float PlayerSpeed = 350;
  inline static const int DefaultMaxBullets = 1;
  inline static const Vector2f DefaultPlayerSizePercent = Vector2f( .05 );
  inline static const float DefaultPlayerBulletSpeed = 500;
  Player( const RectF &rectangle );
  void update( float t ) override;
  void die() override;
  
  void tryShoot();
  void setMovingLeft();
  void setMovingRight();
  
};
