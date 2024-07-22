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

RectF Particle::getDrawBox() const {
  // the pos is the CENTER.
  // +---+
  // | * |
  // +---+
  // 
  RectF drawBox = box;
  drawBox.pos -= drawBox.size / 2;
  return drawBox;
}

void Particle::update( float t ) {
  Sprite::update( t );
  
  pulseTime += t;
  if( pulseTime > pulsePeriod ) {
    pulseTime = 0;
    growing = -growing;
  }
  
  lifeRemaining -= t;
  if( lifeRemaining < 0 ) {
    die();
  }
  
  // Fade the particles.
  float percentLife = lifeRemaining / initialLife;
  for( Animation::Frame &frame : animation.frames ) {
    // pulse the size.
    
    frame.color.a = percentLife * 255;
    
    // Grow/shrink the particle.
    box.size += growing * growthFactor;
  }
}

void Particle::die() {
  if( dead ) {
    error( "%s double die call", name.c_str() );
  }
  
  // mark for removal at end of frame.
  // The reason for doing this is it is awkward to remove from collections while iterating
  // (which is often when we discover the object died)
  dead = 1;
}
