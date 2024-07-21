#include "Particle.h"

Particle::Particle( const RectF &rectangle, float lifetime ) :
    Sprite( rectangle ), initialLife( lifetime ), lifeRemaining( lifetime ) {
  
  vel = Vector2f::random( -100, 100 );
  
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
  
  box.pos += vel * t;
  lifeRemaining -= t;
  
  pulseTime += t;
  if( pulseTime > pulsePeriod ) {
    pulseTime = 0;
    growing = -growing;
  }
  
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
