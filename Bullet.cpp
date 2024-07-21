#include "Bullet.h"

#include "SDLWindow.h"


Bullet::Bullet( const RectF &shooterBounds, bool shotFromInvader ) : fromInvader( shotFromInvader ) {
	name = "Bullet/" + name;
  
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
  
  animation = sdl->getAnimation( AnimationId::InvaderBullet1 );
}

void Bullet::update( float t ) {
  Sprite::update( t );
  
	box.pos += vel * t;
	
	if( box.pos.y < 0 ) {
    die();
  } 
}
