#include "Bullet.h"

#include "SDLWindow.h"

void Bullet::clearDead() {
  allBullets.erase( std::remove_if( allBullets.begin(), allBullets.end(), []( Bullet *bullet ) {
    return bullet->dead;
  } ) );
}

Bullet::Bullet( const RectF &playerBox ) {
	name = "Bullet/" + name;
  
  float windowHeight = sdl->getWindowSize().y;
  
  box.w = windowHeight * .01;
  box.h = windowHeight * .05;
  
  box.x = playerBox.midX() - box.w/2;
  box.y = playerBox.top();
  
  addAnimationFrame( 0, Yellow, .2 );
  addAnimationFrame( 0, Red, .2 );
  
  allBullets.push_back( this );
}

Bullet::~Bullet() {
  
}

void Bullet::update() {
  //Sprite::update();
  animation.update( 1./60 ); // Only update the animation, but don't enforce world limits like for other sprites.
  
	box.y -= 5;
	
	if( box.y < 0 ) {
    die();
  } 
}
