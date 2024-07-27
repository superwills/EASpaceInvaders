#pragma once

#include "Sprite.h"
#include "StlUtil.h"

class Item : public Sprite {
  inline static const Vector2f DefaultSizePercent = Vector2f( .05, .05 );
  
public:
  Item( const Vector2f &center, AnimationId animationId );
  shared_derived( Item )
  
  void update( float t ) override;
  void die() override;
  void onHit( ICollideable *o ) override;
  
};
