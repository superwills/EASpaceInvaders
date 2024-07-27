#pragma once

#include "BulletType.h"
#include "Sprite.h"

class Bullet;
class Item;

class Player : public Sprite {
  inline static const float DefaultPlayerSpeed = 350;
  inline static const Vector2f DefaultPlayerSizePercent = Vector2f( .05 );
  inline static const float DefaultPlayerBulletSpeed = 500;
  int lives = 3;
  inline static const int DefaultMaxBullets = 1;
  int maxBullets = DefaultMaxBullets;
  BulletType weapon = BulletType::PlayerNormal;
  
public:
  bool shielded = 0;
  
  Player();
  void update( float t ) override;
  void die() override;
  
  inline int getMaxBullets() const { return maxBullets; }
  bool canPlayerRespawn();
  inline int getNumLivesRemaining() const { return lives; }
  inline bool isOutOfLives() const { return lives <= 0; }
  void respawn();
  
  void addShield();
  void loseShield();
  
  void onHit( ICollideable *o ) override;
  
  void giveItem( shared_ptr<Item> item );
  void tryShoot();
  void setMovingLeft();
  void setMovingRight();
  
};
