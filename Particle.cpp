#include "Particle.h"

Particle::Particle( const RectF &rectangle, float lifetime ) :
    Sprite( rectangle ), initialLife( lifetime ), lifeRemaining( lifetime ) {
  
  name = "Particle/" + name;
  velocity = Vector2f::random( -100, 100 );
  
  // create a random color cycle animation
  for( int i = 0; i < 15; i++ ) {
    addAnimationFrame( 0, SDL_RandomSolidColor(), .05 );
  }
  
}

void Particle::update( float t ) {
  Sprite::update( t );
  
  pulseTime += t;
  if( pulseTime > pulsePeriod ) {
    pulseTime = 0;
    growthSign = -growthSign;
  }
  // Grow/shrink the particle.
  box.pad( growthSign * growthFactor ); // pulse the size.
  
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

void Particle::die() {
  if( dead ) {
    error( "%s double die call", name.c_str() );
  }
  
  dead = true;
}
