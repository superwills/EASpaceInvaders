#include "Item.h"

Item::Item( const Vector2f &center, AnimationId animationId ) {
  box.size = sdl->getWindowSize() * DefaultSizePercent;
  box.setCenter( center );
  
  velocity = Vector2f( 0, 50 );
  
  setAnimation( animationId );
}
