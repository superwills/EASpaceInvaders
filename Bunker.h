#pragma once

#include <memory>
using std::shared_ptr;

#include "RectF.h"

class Sprite;

// A bunker contains 8 sprites in this shape:
// *** 
// ***
// * *
class Bunker {
public:
  vector< shared_ptr<Sprite> > pieces;
  Bunker( const RectF &bounds );
  void update( float t );
  void draw();
  
  // Kills any pieces of the bunker hit by Sprite.
  bool killHit( shared_ptr<Sprite> other );
  void clearDead();
};
