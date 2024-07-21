#include "Invader.h"

#include "Game.h"

Invader::Invader( const RectF& rectangle, AnimationId animationId ) : Sprite( rectangle, animationId ) {
  name = makeString( "Invader/%s", name.c_str() ); // prepend Invader.
  animation = sdl->getAnimation( animationId );
}

void Invader::update( float t ) {

  animation.update( t );

}
