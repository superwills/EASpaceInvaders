#include "Invader.h"

Invader::Invader( const RectF& rectangle ) : Sprite( rectangle ) {
  name = makeString( "Invader %d", spriteId );
  
  color = SDL_RandomSolidColor();
}
