#include "Particle.h"

Particle::Particle( const RectF &rectangle, float lifetime ) :
    Sprite( rectangle ), initialLife( lifetime ), lifeRemaining( lifetime ) {
  
  vel = Vector2f::random( -100, 100 );
  
  // create a random color cycle animation
  for( int i = 0; i < 15; i++ ) {
    addAnimationFrame( 0, SDL_RandomSolidColor(), .05 );
  }
  
}

void Particle::update( float t ) {
  Sprite::update( t );
  
  box.xy() += vel * t;
  lifeRemaining -= t;
  
  if( lifeRemaining < 0 ) {
    die();
  }
  
  // Fade the particles.
  float percentLife = lifeRemaining / initialLife;
  for( Animation::Frame &frame : animation.frames ) {
    frame.color.a = percentLife * 255;
  }
  
}
