#include "Invader.h"

#include "Game.h"

void Invader::clearDead() {
  allInvaders.erase( std::remove_if( allInvaders.begin(), allInvaders.end(), []( Invader *invader ) {
    return invader->dead;
  } ), allInvaders.end() );
}

Invader::Invader( const RectF& rectangle ) : Sprite( rectangle ) {
  name = makeString( "Invader/%s", name.c_str() ); // prepend Invader.
  
  addAnimationFrame( 0, Blue, .1 );
  addAnimationFrame( 0, Green, .1 );
  addAnimationFrame( 0, Yellow, .1 );
  
  allInvaders.push_back( this );
}


Invader::~Invader() {
  game->changeScore( scoreValue );
  
  
}
