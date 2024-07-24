#include "ScoreDisplay.h"

ScoreDisplay::ScoreDisplay( int value, const Vector2f &center, SDL_Color color ) {
  box.size = sdl->getWindowSize() * Vector2f( .08, .03 );
  box.setCenter( center );
  
  shared_ptr<Texture> tex = sdl->makeTextTexture( makeString( "%d", value ) );
  addAnimationFrame( tex, color );
  
  minParticles = maxParticles = 0;
  velocity.y = -5;
}

void ScoreDisplay::update( float t ) {
  Sprite::update( t );
  
  timeAlive += t;
  
  if( timeAlive >= MaxLifetime ) {
    die();
  }

}
