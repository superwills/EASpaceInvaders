#pragma once

#include "Sprite.h"

class Bullet;

class Player : public Sprite {
public:
  Player( const RectF &rectangle );
  void die() override;
};
