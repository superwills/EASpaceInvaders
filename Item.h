#pragma once

#include "Sprite.h"

#define shared_derived( CLASS ) shared_ptr<CLASS> shared_##CLASS() { return static_pointer_cast<CLASS>( shared_from_this() ); }

class Item : public Sprite {
  inline static const Vector2f DefaultSizePercent = Vector2f( .05, .05 );
  
public:
  Item( const Vector2f &center, AnimationId animationId );
  //shared_ptr<Item> shared_Item() { return dynamic_pointer_cast<Item>( shared_from_this() ); }
  shared_derived( Item )
  
  void onHit( ICollideable *o ) override;
  
};
