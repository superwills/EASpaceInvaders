#pragma once

#include "Sprite.h"
#include "Vectorf.h"

class Particle : public Sprite {
public:
  inline static float DefaultLifetime = 1;
  
  float initialLife = DefaultLifetime;
  float lifeRemaining = DefaultLifetime;
  Vector2f vel;
  int growing = 1;        // when growing=1, shrinking=-1
  float growthFactor = .2;
  float pulsePeriod = .1; // Grow/shrink cycle every (this) seconds.
  float pulseTime = 0;
  
  Particle( const RectF &rectangle, float lifetime = DefaultLifetime );
  
  // Draws from center.
  RectF getDrawBox() const override;
  
  // decay the particle alpha
  void update( float t ) override;
};
