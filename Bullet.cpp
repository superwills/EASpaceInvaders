#include "Bullet.h"

#include "SDLWindow.h"

bool Bullet::IsBulletTypeFromInvader( BulletType bulletType ) {
  bool isInvader = isBetweenIn( bulletType, BulletType::InvaderNormal, BulletType::InvaderSuper );
  
  info( "Bullet %d is from %s", bulletType, isInvader?"Invader":"Player" ) ;
  
  return isInvader; 
}

Bullet::Bullet( const Vector2f &shootCenter, BulletType bulletType ) :
    type( bulletType ) {
  
  name = "Bullet/" + name;
  updateAnimationType();
  
  velocity.y = getBulletSpeed();
  
  // Hitbox has to be a lot narrower for bullets, so they don't hit as easily but still remain easy visible
  hitBoxScale.x = .2;

  if( type == BulletType::InvaderSuper ) {
    hitBoxScale.x = .5; // lightning has a wide hit box
  }
  
  float windowHeight = sdl->getWindowSize().y;
  box.size = Vector2f( windowHeight ) * DefaultBulletScale;
  box.setCenter( shootCenter );
  
  minParticles = 3, maxParticles = 6;
  particleSizeMin = 2, particleSizeMax = 4;
}

void Bullet::update( float t ) {
  Sprite::update( t );
  
  // Once it leaves the bounds kill the bullet
	if( !sdl->getWindowRectangle().hit( box ) ) {
    die();
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
