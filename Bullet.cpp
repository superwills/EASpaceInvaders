#include "Bullet.h"

#include "SDLWindow.h"

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
  allBullets.erase( std::remove( allBullets.begin(), allBullets.end(), this ) );
}

void Bullet::update() {
  Sprite::update();
	box.y--;
	
	if( box.y < 0 ) {
    die();
  } 
}
