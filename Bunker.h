#pragma once

#include <memory>
using std::shared_ptr;

#include "ICollideable.h"
#include "RectF.h"

class BunkerPiece;
class Sprite;

// A bunker contains 8 sprites in this shape:
//   *** 
//   ***
//   * *
class Bunker : public ICollideable {
  RectF bounds;
  vector< shared_ptr<BunkerPiece> > pieces;
  
public:
  Bunker( const RectF &initialBounds );
  void update( float t );
  void draw();
  
  RectF getHitBox() const override {  return bounds;  }
  Vector2f getCentroid() const override {  return bounds.centroid();  }
  inline string getName() const override {  return makeString( "Bunker %s", bounds.toString().c_str() );  }
  
  // Considered dead when it has no pieces left.
  inline bool isDead() const { return pieces.empty(); } 
  
  bool checkPiecesHit( shared_ptr<ICollideable> o );
  
  void onHit( ICollideable *o ) override;
  
  void clearDead();
};
