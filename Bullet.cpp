#include "Bullet.h"

#include "SDLWindow.h"


Bullet::Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool shotFromInvader ) :
    fromInvader( shotFromInvader ) {
  
  name = "Bullet/" + name;
  velocity = initialVelocity;
	
  float windowHeight = sdl->getWindowSize().y;
  box.size = Vector2f( windowHeight * .01, windowHeight * .05 );
  
  box.pos.x = shooterBounds.midX() - box.size.x/2;
  if( fromInvader ) {
    // invader shoots from bottom
    box.pos.y = shooterBounds.bottom();
  }
  else {
    // Player shoots from top
    box.pos.y = shooterBounds.top(); 
  }
  
  if( shotFromInvader ) {
    animation = sdl->getAnimation( rand<AnimationId>( AnimationId::InvaderBullet1, AnimationId::InvaderBullet2 ) );
  }
  else {
    // player bullet is solid
    addSolidColorAnimationFrame( Yellow );
    addSolidColorAnimationFrame( Red );
  }
}

void Bullet::update( float t ) {
  Sprite::update( t );
  
	if( box.pos.y < 0 ) {
    die();
  } 
}
