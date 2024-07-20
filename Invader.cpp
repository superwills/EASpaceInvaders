#include "Invader.h"

#include "Game.h"

Invader::Invader( const RectF& rectangle ) : Sprite( rectangle ) {
  name = "Invader/" + name; // prepend Invader.
  addAnimationFrame( 0, HUGEF );
}


Invader::~Invader() {
  game->changeScore( scoreValue );
}
