#include "BulletLaser.h"

#include "Game.h"

BulletLaser::BulletLaser( const Vector2f &shootCenter ) : Bullet( shootCenter, BulletType::PlayerThickLaser ) {
  particleCloudProperties.setNumParticles( 50, 60 );
  particleCloudProperties.setSizes( 24, 36 );
  particleCloudProperties.initialParticleDecay = .2;
  game->shakeScreen( .25 );
}

void BulletLaser::update( float t ) {
  Bullet::update( t );
  
  // grow the box
  float laserGrowthSpeed = 50;
  box.size.y += laserGrowthSpeed;
  box.pos.y -= laserGrowthSpeed;
  
  // laser dies once it reaches the top
  if( box.top() < 0 ) {
    // it can spread particles along it
    die();
  }
  
}
