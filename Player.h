#pragma once

#include "Sprite.h"

class Bullet;

class Player : public Sprite {
  inline static const float DefaultPlayerSpeed = 350;
  inline static const Vector2f DefaultPlayerSizePercent = Vector2f( .05 );
  inline static const float DefaultPlayerBulletSpeed = 500;
  int lives = 3;
  
public:
  inline static const int DefaultMaxBullets = 1;
  
  Player();
  void update( float t ) override;
  void die() override;
  
  bool canPlayerRespawn();
  inline int getNumLivesRemaining() const { return lives; }
  inline bool isOutOfLives() const { return lives <= 0; }
  void respawn();
  
  void tryShoot();
  void setMovingLeft();
  void setMovingRight();
  
};
