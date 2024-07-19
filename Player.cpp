#include "Player.h"

Player::Player( Vector2f size, float iSpeed ) {
      
  box.w = size.x;
  box.h = size.y;
	speed = iSpeed;
}

void Player::moveDown() {
  box.y += speed;
	bounceTopAndBottom();
}

void Player::moveUp() {
  box.y -= speed;
	bounceTopAndBottom();
}
