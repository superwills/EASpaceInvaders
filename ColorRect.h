#pragma once

#include "RectF.h"
#include "SDLColors.h"

struct ColorRect {
  RectF bounds;
  SDL_Color color = Red;
  bool fill = 0;
  
  inline static const int DefaultNumFrames = 10;
  int frames = DefaultNumFrames;
  
  inline bool isDead() const { return frames <= 0; }
  ColorRect( const RectF &rect, SDL_Color theColor, int numFrames = DefaultNumFrames ) :
    bounds( rect ), color( theColor ), frames( numFrames ) {
  }
};


