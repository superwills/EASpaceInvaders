#pragma once

#include "Sprite.h"
#include "Assets.h"

class Invader : public Sprite {
  
public:
  int scoreValue = 1;
  AnimationId character = AnimationId::Invader1;
  inline static map<AnimationId, int> scoreForCharacter = {
    { AnimationId::Invader1, 10 },
    { AnimationId::Invader2, 25 },
    
    { AnimationId::E, 75 },
    { AnimationId::A, 50 },
    
    { AnimationId::UFO, 100 },
  };
  
  // Must specify where/size of Invader to construct
  Invader( const RectF& rectangle, AnimationId animationId );
  
  int getScore() const;
};
