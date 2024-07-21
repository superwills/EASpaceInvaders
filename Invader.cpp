#include "Invader.h"

#include "Game.h"
#include "Log.h"

Invader::Invader( const RectF& rectangle, AnimationId animationId ) : Sprite( rectangle, animationId ) {
  name = makeString( "Invader/%s", name.c_str() ); // prepend Invader.
  animation = sdl->getAnimation( animationId );
}

