#pragma once

#include "Sprite.h"

class Bullet : public Sprite {
public:
  inline static Vector2f bulletScale = Vector2f( .02, .03 );
  bool isFromInvader = 0;
  
  Bullet( const RectF &shooterBounds, const Vector2f &initialVelocity, bool isShotFromInvader );
  void update( float t ) override;
};
