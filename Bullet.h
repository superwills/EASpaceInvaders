#pragma once

#include "Sprite.h"

class Bullet : public Sprite {
public:
  inline static Vector2f bulletScale = Vector2f( .002, .03 );
  bool fromInvader = 0;
  
  Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool shotFromInvader );
  void update( float t ) override;
};
