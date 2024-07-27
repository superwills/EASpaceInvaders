#pragma once

#include "Sprite.h"

class UFO : public Sprite {
  int sfxChannel = -1;
  inline static const float DefaultUFOSpeed = 200;
  inline static const Vector2f DefaultSizePercent = Vector2f( .08, .06 );
public:
  inline static float GenInterval = 15;
  
  UFO();
  ~UFO();
  
  void update( float t ) override;
  void onHit( ICollideable *o ) override;
  
  int getScore() const override;
};
