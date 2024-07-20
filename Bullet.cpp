#include "Bullet.h"

#include "SDLWindow.h"

Bullet::Bullet( const RectF &playerBox ) {
	name = "Bullet/" + name;
  
  float windowHeight = sdl->getWindowSize().y;
  
  box.x = playerBox.midX();
  box.y = playerBox.top();
  
  box.w = windowHeight * .01;
  box.h = windowHeight * .05;
  
  allBullets.push_back( this );
}

Bullet::~Bullet() {
  allBullets.erase( std::remove( allBullets.begin(), allBullets.end(), this ) );
}

void Bullet::update() {
	box.y--;
	
	if( box.y < 0 ) {
    die();
  } 
}
