#include "UFO.h"

#include "Assets.h"

UFO::UFO( const RectF &rectangle ) : Sprite( rectangle, AnimationId::UFO ) {

}

void UFO::update( float t ) {
  Sprite::update( t );
  
  box.pos.x -= 5;
  
  
}
