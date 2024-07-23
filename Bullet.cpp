#include "Bullet.h"

#include "SDLWindow.h"


Bullet::Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool isShotFromInvader ) :
    isFromInvader( isShotFromInvader ) {
  
  name = "Bullet/" + name;
  velocity = initialVelocity;
	
  float windowHeight = sdl->getWindowSize().y;
  box.size = Vector2f( windowHeight ) * bulletScale;
  
  box.pos.x = shooterBounds.midX() - box.size.x/2;
  if( isFromInvader ) {
    // invader shoots from bottom
    box.pos.y = shooterBounds.bottom();
  }
  else {
    // Player shoots from top
    box.pos.y = shooterBounds.top(); 
  }
  
  if( isFromInvader ) {
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
  
  // Once it leaves the bounds kill the bullet
	if( !sdl->getWindowRectangle().hit( box ) ) {
    die();
  } 
}
