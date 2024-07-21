#pragma once

#include "Sprite.h"
#include "Assets.h"

class Invader : public Sprite {
  
public:
  // Must specify where/size of Invader to construct
  Invader( const RectF& rectangle, AnimationId animationId );
  
  void update( float t ) override;
};
