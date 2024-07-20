#include "Player.h"

Player::Player( const Vector2f &windowSize ) {
  name = "Player/" + name;
  
  float playerWidth = windowSize.x * .05; // 5% of worldWidth
  box.w = box.h = playerWidth;
  
  // Set initial positions for player paddles
  float hCenter = windowSize.y/2;
  
  setCenter( hCenter, windowSize.y - box.h/2 );
  
  //addBlankAnimationFrame();
  addAnimationFrame( 0, Red, 1 );
  addAnimationFrame( 0, Green, 1 );
}

void Player::move( float x ) {
  box.x += x;
	enforceWorldLimits();
}

