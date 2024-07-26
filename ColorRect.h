#pragma once

#include "RectF.h"
#include "SDLColors.h"

struct ColorRect {
  RectF bounds;
  SDL_Color color = Red;
  bool fill = 0;
  
  ColorRect( const RectF &rect, SDL_Color theColor ) :
    bounds( rect ), color( theColor ) {
  } 
};


