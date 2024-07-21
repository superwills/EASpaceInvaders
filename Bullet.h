#pragma once

#include "Sprite.h"

class Bullet : public Sprite {
public:
  Vector2f vel;
  bool fromInvader = 0;
  
  Bullet( const RectF &shooterBounds, bool shotFromInvader );
  void update( float t ) override;
};
