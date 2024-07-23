#include "Player.h"

#include "Bullet.h"
#include "Game.h"
#include "SDLWindow.h"

Player::Player() {
  name = "Player/" + name;
  
  Vector2f windowSize = sdl->getWindowSize();
  box.size = windowSize * Player::DefaultPlayerSizePercent;
  
  Vector2f playerCenter( windowSize.x/2, windowSize.y - box.size.y/2 );
  box.setCenter( playerCenter );
  
  animation = sdl->getAnimation( AnimationId::Player );
  
  deathSound = SFXId::ExplodePlayer;
  
  minParticles = 25;
  maxParticles = 50;
  particleSizeMin = 30;
  particleSizeMax = 50;
}

void Player::update( float t ) {
  Sprite::update( t );
  velocity.x = 0;   // player kill vel each frame
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
  velocity.x = -DefaultPlayerSpeed;
}

void Player::setMovingRight() {
  velocity.x = +DefaultPlayerSpeed;
}
