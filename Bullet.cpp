#include "Bullet.h"

#include "Game.h"
#include "SDLWindow.h"

bool Bullet::IsBulletTypeFromInvader( BulletType bulletType ) {
  return isBetweenIn( bulletType, BulletType::InvaderNormal, BulletType::InvaderSuper );; 
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
  
  if( type == BulletType::PlayerThickLaser ) {
    minParticles = maxParticles = 0;
    game->shakeScreen( .25 );
  }
}

void Bullet::update( float t ) {
  Sprite::update( t );
  
  if( type == BulletType::PlayerThickLaser ) {
    // grow the box
    float laserGrowthSpeed = 50;
    box.size.y += laserGrowthSpeed;
    box.pos.y -= laserGrowthSpeed;
    
    // laser dies once it reaches the top
    if( box.top() < 0 ) {
      // it can spread particles along it
      game->particleCloud( box, randInt( 50, 60 ), 24, 36, .2 );
      die();
    }
  }
  
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
