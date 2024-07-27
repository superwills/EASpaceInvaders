#include "Bullet.h"

#include "Game.h"
#include "SDLWindow.h"

bool Bullet::IsBulletTypeFromInvader( BulletType bulletType ) {
  return isBetweenIn( bulletType, BulletType::InvaderNormal, BulletType::InvaderSuper );; 
}

Bullet::Bullet( const Vector2f &shootCenter, BulletType bulletType ) :
    type( bulletType ) {
  
  name = "Bullet/" + name;
  collisionType = ICollideableType::Bullet;
  updateAnimationType();
  
  velocity.y = getBulletSpeed();
  
  // Hitbox has to be a lot narrower for bullets, so they don't hit as easily but still remain easy visible
  hitBoxScale.x = .4;

  if( type == BulletType::InvaderSuper ) {
    hitBoxScale.x = .5; // lightning has a wide hit box
  }
  if( type == BulletType::PlayerThickLaser ) {
    hitBoxScale.x = .9;
  }
  
  float windowHeight = sdl->getWindowSize().y;
  box.size = Vector2f( windowHeight ) * DefaultBulletScale;
  box.setCenter( shootCenter );
  
  particleCloudProperties.setNumParticles( 3, 6 );
  particleCloudProperties.setSizes( 2, 4 );
}

void Bullet::update( float t ) {
  Sprite::update( t );
  
  // Once it leaves the bounds kill the bullet
	if( !sdl->getWindowRectangle().hit( box ) ) {
    die();
  }
}

void Bullet::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet: {
      Bullet *bullet = (Bullet*)o;
      
      // they have to be from opposite teams to collide
      if( isFromInvader() != bullet->isFromInvader() ) {
        die();
      }
    }
    break;
  case ICollideableType::Bunker:
    break;
  case ICollideableType::BunkerPiece:
    die();
    break;
  case ICollideableType::Invader:
    die();
    break;
  case ICollideableType::Item:
    break;
  case ICollideableType::Player:
    die();
    break;
  case ICollideableType::UFO:
    die();
    break;
    
  case ICollideableType::NotCollideable:
  default:
    error( "Colliding with non-collideable" );
    break;
  }
}


void Bullet::updateAnimationType() {
  auto it = BulletAnimations.find( type );
  if( it == BulletAnimations.end() ) {
    error( "No animation for %d", type );
    return;
  }
  
  setAnimation( it->second );
}

float Bullet::getBulletSpeed() const {
  auto it = BulletSpeeds.find( type );
  if( it == BulletSpeeds.end() ) {
    error( "No such bullet %d", type );
    return 100;
  }
  
  return it->second;
}

bool Bullet::isFromInvader() const {
  return IsBulletTypeFromInvader( type );
}
