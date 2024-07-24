#pragma once

#include "Sprite.h"
#include "Vectorf.h"

class Particle : public Sprite {
  inline static float DefaultLifetime = 1;
  
  float initialLife = DefaultLifetime;
  
  int growthSign = 1;        // when growing=1, shrinking=-1
  float growthFactor = .2;
  float pulsePeriod = .1; // Grow/shrink cycle every (this) seconds.
  float pulseTime = 0;
  
public:
float lifeRemaining = DefaultLifetime;
  Particle( const RectF &rectangle, float lifetime = DefaultLifetime );
  
  // decay the particle alpha
  void update( float t ) override;
  
  // Prevent particles from spawning other particles!
  void die() override;
};
