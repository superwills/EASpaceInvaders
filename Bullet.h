#pragma once

#include "Sprite.h"

class Bullet : public Sprite {
public:
  Bullet( const RectF &playerBox );
  void update() override;
};
