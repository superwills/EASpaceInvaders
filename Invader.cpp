#include "Invader.h"

#include "Game.h"

Invader::Invader( const RectF& rectangle ) : Sprite( rectangle ) {
  name = makeString( "Invader %d", spriteId );
  
  color = SDL_RandomSolidColor();
}

Invader::~Invader() {
  game->changeScore( scoreValue );
}
