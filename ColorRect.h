#pragma once

#include "RectF.h"
#include "SDLColors.h"

struct ColorRect {
  RectF bounds;
  SDL_Color color = Red;
  bool fill = 0;
  
  inline static const int DefaultLifetime = 10;
  float initialLifetime = DefaultLifetime;
  float lifetime = DefaultLifetime;
  
  inline bool isDead() const { return lifetime <= 0; }
  void update( float t ) {
    lifetime -= t;
    float percentLifeRem = lifetime / initialLifetime;
    color.a = 255 * percentLifeRem;
  }
  ColorRect( const RectF &rect, SDL_Color theColor, float startingLifetime = DefaultLifetime ) :
    bounds( rect ), color( theColor ), initialLifetime( startingLifetime ), lifetime( startingLifetime ) {
  }
};


