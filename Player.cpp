#include "Player.h"

#include "Bullet.h"
#include "SDLWindow.h"

Player::Player( const RectF &rectangle ) : Sprite( rectangle, AnimationId::Player ) {
  name = "Player/" + name;
  
  deathSound = SFXId::ExplodePlayer;
}

void Player::update( float t ) {
  Sprite::update( t );

  // player kill vel each frame
  velocity.x = 0;
}

void Player::die() {
  Sprite::die();
  // swap out the player death sprite.
  animation = sdl->getAnimation( AnimationId::PlayerDie );
}

void Player::setMovingLeft() {
  velocity.x = -PlayerSpeed;
}

void Player::setMovingRight() {
  velocity.x = +PlayerSpeed;
}
