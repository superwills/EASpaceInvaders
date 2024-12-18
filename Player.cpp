#include "Player.h"

#include "Bullet.h"
#include "Game.h"
#include "Item.h"
#include "SDLWindow.h"

Player::Player() {
  name = "Player/" + name;
  collisionType = ICollideableType::Player;
  
  Vector2f windowSize = sdl->getWindowSize();
  box.size = windowSize * Player::DefaultPlayerSizePercent;
  
  Vector2f playerCenter( windowSize.x/2, windowSize.y - box.size.y/2 );
  box.setCenter( playerCenter );
  
  animation = sdl->getAnimation( AnimationId::Player );
  
  deathSound = SFXId::ExplodePlayer;
  
  particleCloudProperties.setNumParticles( 25, 40 );
  particleCloudProperties.setSizes( 16, 22 );
  particleCloudProperties.initialDecay = .8;
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
  
  game->playerDie();
}

bool Player::canPlayerRespawn() {
  return dead && animation.isEnded() && !isOutOfLives();
}

void Player::respawn() {
  maxBullets = DefaultMaxBullets;
  setAnimation( AnimationId::Player );
  weapon = BulletType::PlayerNormal;
  dead = 0;
}

void Player::addShield() {
  if( shielded ) {
    return; // player already shielded
  }
  
  shielded = true;
  RectF shieldBox = box;
  shieldBox.pad( box.size/2 );
  auto shieldSprite = std::make_shared<Sprite>( shieldBox, AnimationId::Shield );
  children.push_back( shieldSprite );
}

void Player::loseShield() {
  // The player won't die if shielded, but the shield drops.
  shielded = 0;
  children.clear(); // drops the shield.
}

void Player::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet: {
      Bullet *bullet = (Bullet*)o;
      
      if( bullet->isFromInvader() ) {
        if( shielded ) {
          loseShield();
        }
        else {
          die();
        }
      }
    }
    break;
  case ICollideableType::Bunker:
    break;
  case ICollideableType::BunkerPiece:
    break;
  case ICollideableType::Invader:
    die();
    break;
  case ICollideableType::Item: {
      auto item = ((Item*)o)->shared_Item();
      giveItem( item );
    }
    break;
  case ICollideableType::Player:
    break;
  case ICollideableType::UFO:
    break;
    
  case ICollideableType::NotCollideable:
  default:
    error( "Colliding with non-collideable" );
    break;
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
    
  default:
    break;
  }
  
  sdl->playSound( SFXId::Blip );
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
