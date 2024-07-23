#pragma once

#include "Sprite.h"
#include "Assets.h"

class InvaderGroup;

class Invader : public Sprite {
  friend class InvaderGroup;
  inline static Vector2f DefaultBulletSpeed = Vector2f( 0, +100 );
  
  // Starts low, but InvaderGroup increases it's value as invaders die off
  inline static const float DefaultChanceToShoot = .1;
  inline static float ChanceToShoot = DefaultChanceToShoot;
  inline static float DefaultSpeed = 50;
  inline static float Speed = DefaultSpeed;
  
public:
  Invader( const RectF& rectangle, AnimationId animationId );
  
  void update( float t ) override;
};
