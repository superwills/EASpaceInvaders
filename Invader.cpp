#include "Invader.h"

#include "Game.h"

Invader::Invader( const RectF& rectangle ) : Sprite( rectangle ) {
  name = "Invader/" + name; // prepend Invader.
  addAnimationFrame( 0, Blue, .1 );
  addAnimationFrame( 0, Green, .1 );
}


Invader::~Invader() {
  game->changeScore( scoreValue );
}
