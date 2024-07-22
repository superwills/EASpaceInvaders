#include "Invader.h"

#include "Game.h"
#include "Log.h"

Invader::Invader( const RectF& rectangle, AnimationId animationId ) : Sprite( rectangle, animationId ) {
  name = makeString( "Invader/%s", name.c_str() ); // prepend Invader.
  animation = sdl->getAnimation( animationId );
}

void Invader::update( float t ) {
  Sprite::update( t );
  // shoot with chance.
  if( randFloat() < .0001 ) {
    game->shootBullet( box, 1, Vector2f( 0, 100 ) );
  }
}
