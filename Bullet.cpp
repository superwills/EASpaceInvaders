#include "Bullet.h"

#include "SDLWindow.h"


Bullet::Bullet( const RectF &playerBox ) {
	name = "Bullet/" + name;
  
  float windowHeight = sdl->getWindowSize().y;
  
  box.size = Vector2f( windowHeight * .01, windowHeight * .05 );
  
  box.pos.x = playerBox.midX() - box.size.x/2;
  box.pos.y = playerBox.top();
  
  addAnimationFrame( 0, Yellow, .2 );
  addAnimationFrame( 0, Red, .2 );
}

void Bullet::update( float t ) {
  //Sprite::update();
  animation.update( t ); // Only update the animation, but don't enforce world limits like for other sprites.
  
	box.pos.y -= 5;
	
	if( box.pos.y < 0 ) {
    die();
  } 
}
