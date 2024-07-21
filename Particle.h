#pragma once

#include "Sprite.h"
#include "Vectorf.h"
class Particle : public Sprite {
public:
  inline static float DefaultLifetime = 5;
  
  float initialLife = DefaultLifetime;
  float lifeRemaining = DefaultLifetime;
  Vector2f vel;
  
  Particle( const RectF &rectangle, float lifetime = DefaultLifetime );
  
  // decay the particle alpha
  void update( float t ) override;
};
