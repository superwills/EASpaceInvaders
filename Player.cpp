#include "Player.h"

#include "Bullet.h"
#include "SDLWindow.h"

Player::Player( const Vector2f &windowSize ) {
  name = "Player/" + name;
  
  float playerWidth = windowSize.x * .05; // 5% of worldWidth
  box.size = playerWidth;
  
  // Set initial positions for player
  float hCenter = windowSize.x/2;
  box.pos.x = hCenter - box.size.x/2;
  box.pos.y = windowSize.y - box.size.y;
  
  //addBlankAnimationFrame();
  addAnimationFrame( 0, Red, 1 );
  addAnimationFrame( 0, Green, 1 );
}

shared_ptr<Bullet> Player::shoot() {
  return std::make_shared<Bullet>( box );
}

