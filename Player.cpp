#include "Player.h"

#include "Bullet.h"
#include "Game.h"
#include "Item.h"
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
  
  if( canPlayerRespawn() ) {
    respawn();
  }
}

void Player::die() {
  lives--;
  Sprite::die();
  // swap out the player death sprite.
  setAnimation( AnimationId::PlayerDie );
}

bool Player::canPlayerRespawn() {
  return dead && animation.isEnded() && !isOutOfLives();
}

void Player::respawn() {
  maxBullets = DefaultMaxBullets;
  setAnimation( AnimationId::Player );
  dead = 0;
}

void Player::addShield() {
  if( shielded ) {
    return; // player already shielded
  }
  
  shielded = 1;
  RectF shieldBox = box;
  shieldBox.pad( box.size/2 );
  auto shieldSprite = std::make_shared<Sprite>( shieldBox, AnimationId::Shield );
  children.push_back( shieldSprite );
}

void Player::loseShield() {
  if( shielded ) {
    // The player won't die if shielded, but the shield drops.
    shielded = 0;
    children.clear(); // drops the shield.
  }
}

void Player::giveItem( shared_ptr<Item> item ) {
  switch( item->character ) {
  case AnimationId::ItemPlus1:
    maxBullets++;
    break;
    
  case AnimationId::ItemShield:
    addShield();
    break;
    
  case AnimationId::ItemSpread:
    weapon = BulletType::PlayerSpread;
    break;
    
  case AnimationId::ItemThickLaser:
    weapon = BulletType::PlayerThickLaser;
    break;
  }
}

void Player::tryShoot() {
  game->tryShootBullet( weapon, box.midTop() );
}

void Player::setMovingLeft() {
  velocity.x = -DefaultPlayerSpeed;
}

void Player::setMovingRight() {
  velocity.x = +DefaultPlayerSpeed;
}
