#include "Player.h"

Player::Player( const Vector2f &worldSize ) {
  float playerWidth = worldSize.x * .05; // 5% of worldWidth
  box.w = box.h = playerWidth;
}

void Player::move( float x ) {
  box.x += x;
	enforceWorldLimits();
}

