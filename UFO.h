#pragma once

#include "Sprite.h"

class UFO : public Sprite {
  int sfxChannel = -1;
  inline static const float DefaultUFOSpeed = 250;
  
public:
  UFO( const RectF &rectangle );
  ~UFO();
  
  void update( float t ) override;
};
