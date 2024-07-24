#pragma once

#include "Sprite.h"

class Item : public Sprite {
  inline static const Vector2f DefaultSizePercent = Vector2f( .05, .05 );
  
public:
  Item( const Vector2f &center, AnimationId animationId ); 
};
