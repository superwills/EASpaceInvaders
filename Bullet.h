#pragma once

#include "Sprite.h"
#include "Vectorf.h"
#include "RectF.h"

#include <memory>
using std::shared_ptr;

class Bullet : public Sprite {
public:
  Bullet( const RectF &playerBox );
  void update() override;
};
