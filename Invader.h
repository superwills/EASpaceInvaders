#pragma once

#include "Sprite.h"

class Invader : public Sprite {
  
public:
  int scoreValue = 1;
  
  // Must specify where/size of Invader to construct
  Invader( const RectF& rectangle );
  
};
