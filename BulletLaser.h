#pragma once

#include "Bullet.h"

// Laser was different enough from Bullet to warrant subclass
class BulletLaser : public Bullet {

  inline static const float CloudMinParticles = 50, CloudMaxParticles = 60;
  
  BulletLaser( const Vector2f &shootCenter );
  
  void update( float t ) override;
  
  void die() override;
  
};
