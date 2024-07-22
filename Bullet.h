#pragma once

#include "Sprite.h"

class Bullet : public Sprite {
public:
  bool fromInvader = 0;
  
  Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool shotFromInvader );
  void update( float t ) override;
};
