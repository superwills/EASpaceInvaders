#include "ScoreDisplay.h"

ScoreDisplay::ScoreDisplay( int value, const Vector2f &center, SDL_Color color ) {
  string numbers = makeString( "%d", value );
  Vector2f digitSize = sdl->getWindowSize() * Vector2f( .01, .01 ) * numbers.size();
  
  box.size = digitSize * Vector2f( numbers.size(), 1 );
  box.setCenter( center );
  
  shared_ptr<Texture> tex = sdl->makeTextTexture( numbers );
  addAnimationFrame( tex, color );
  
  particleCloudProperties.setNumParticles( 0, 0 );
  velocity.y = -5;
}

void ScoreDisplay::update( float t ) {
  Sprite::update( t );
  
  timeAlive += t;
  
  float percLifeRemain = (MaxLifetime - timeAlive) / MaxLifetime;
  
  for( auto &frame : animation.frames ) {
    frame.color.a = 255 * percLifeRemain;
  }
  
  if( timeAlive >= MaxLifetime ) {
    die();
  }

}
