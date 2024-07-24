#pragma once

#include "Sprite.h"

class ScoreDisplay : public Sprite {
public:
  ScoreDisplay( int value, const Vector2f &center, SDL_Color color );
};
