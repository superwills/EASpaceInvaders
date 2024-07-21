#pragma once

#include "Sprite.h"

class UFO : public Sprite {
public:
  UFO( const RectF &rectangle );
  void update( float t ) override;
  
};
