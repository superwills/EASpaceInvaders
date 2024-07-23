#include "Bullet.h"

#include "SDLWindow.h"


Bullet::Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool isShotFromInvader ) :
    isFromInvader( isShotFromInvader ) {
  
  name = "Bullet/" + name;
  velocity = initialVelocity;
  
  // Hitbox has to be a lot narrower for bullets, so they don't hit as easily but still remain easy visible
  hitBoxScale.x = .2;

  if( isFromInvader ) {
    // superbullets are 2x as fast and have a different graphic
    bool superBullet = 0;
    if( isFromInvader ) {
      superBullet = withChance( .5 );
      hitBoxScale.x = .5; // lightning has a wide hit box
    }
    
    AnimationId anim = AnimationId::BulletInvaderArrow;
    if( superBullet ) {
      velocity *= 3;
      anim = AnimationId::BulletInvaderLightning;
    }
    animation = sdl->getAnimation( anim );
  }
  else {
    animation = sdl->getAnimation( AnimationId::BulletPlayer );
  }
  
  float windowHeight = sdl->getWindowSize().y;
  box.size = Vector2f( windowHeight ) * DefaultBulletScale;
  
  box.pos.x = shooterBounds.midX() - box.size.x/2;
  if( isFromInvader ) {
    // invader shoots from bottom
    box.pos.y = shooterBounds.bottom();
  }
  else {
    // Player shoots from top
    box.pos.y = shooterBounds.top();
  }
  
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
