#pragma once

#include <memory>
using std::shared_ptr;

#include "RectF.h"

class BunkerPiece;
class Sprite;

// A bunker contains 8 sprites in this shape:
//   *** 
//   ***
//   * *
class Bunker {
  RectF bounds;
  vector< shared_ptr<BunkerPiece> > pieces;
  
public:
  Bunker( const RectF &initialBounds );
  void update( float t );
  void draw();
  
  // Considered dead when it has no pieces left.
  inline bool isDead() const { return pieces.empty(); } 
  
  // Kills any pieces of the bunker hit by Sprite.
  bool killHit( shared_ptr<Sprite> other );
  void clearDead();
};
