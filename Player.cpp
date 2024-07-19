#include "Player.h"

Player::Player( Vector2f size, float iSpeed ) {
      
  box.w = size.x;
  box.h = size.y;
	speed = iSpeed;
}

void Player::move( float x ) {
  box.x += x;
	enforceWorldLimits();
}

