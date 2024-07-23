#pragma once

#include "Sprite.h"

class Bullet : public Sprite {
  inline static Vector2f DefaultBulletScale = Vector2f( .02, .03 );

public:
  bool isFromInvader = 0;
  
  Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool isShotFromInvader );
  void update( float t ) override;
};
