#pragma once

#include "Sprite.h"

class ScoreDisplay : public Sprite {
  float timeAlive = 0;
  inline static const float MaxLifetime = 1.5;
public:
  ScoreDisplay( int value, const Vector2f &center, SDL_Color color );
  void update( float t ) override;
};
