#pragma once

#include "Sprite.h"

class Invader : public Sprite {
  int scoreValue = 1;
public:
  
  // Must specify where/size of Invader to construct
  Invader( const RectF& rectangle );
  ~Invader();
  
};
