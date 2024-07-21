#include "Invader.h"

#include "Game.h"
#include "Log.h"

Invader::Invader( const RectF& rectangle, AnimationId animationId ) : Sprite( rectangle, animationId ) {
  name = makeString( "Invader/%s", name.c_str() ); // prepend Invader.
  animation = sdl->getAnimation( animationId );
}

int Invader::getScore() const {
  auto it = scoreForCharacter.find( character );
  
  if( it == scoreForCharacter.end() ) {
    error( "Character %d has no score assigned to it", character );
    return 0;
  }
  
  return it->second;
}
