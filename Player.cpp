#include "Player.h"

#include "Bullet.h"
#include "Game.h"
#include "SDLWindow.h"

Player::Player( const RectF &rectangle ) : Sprite( rectangle, AnimationId::Player ) {
  name = "Player/" + name;
  
  deathSound = SFXId::ExplodePlayer;
  
  minParticles = 25;
  maxParticles = 50;
  particleSizeMin = 30;
  particleSizeMax = 50;
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

void Player::tryShoot() {
  game->tryShootBullet( box, 0, Vector2f( 0, -DefaultPlayerBulletSpeed ) );
}

void Player::setMovingLeft() {
  velocity.x = -PlayerSpeed;
}

void Player::setMovingRight() {
  velocity.x = +PlayerSpeed;
}
