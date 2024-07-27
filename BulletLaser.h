#pragma once

#include "Bullet.h"

// Laser was different enough from Bullet to warrant subclass
class BulletLaser : public Bullet {
public:
  BulletLaser( const Vector2f &shootCenter );
  
  void update( float t ) override;
  
  void onHit( shared_ptr<ICollideable> o ) override;
  
};
