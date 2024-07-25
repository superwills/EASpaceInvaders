#pragma once

#include <memory>
using std::shared_ptr;

#include "Vectorf.h"
#include "RectF.h"

class ICollideable {
public:
  virtual RectF getHitBox() const = 0;
  virtual bool hit( const Vector2f &pt ) {
    bool didHit = getHitBox().hit( pt );
    if( didHit ) {
      onHit();
    }
    return didHit;
  }
  virtual bool hit( const RectF &rect ) {
    bool didHit = getHitBox().hit( rect );
    if( didHit ) {
      onHit();
    }
    return didHit;
  }
  virtual bool hit( shared_ptr<ICollideable> other ) {
    bool didHit = getHitBox().hit( other->getHitBox() );
    if( didHit ) {
      onHit();
    }
    return didHit;
  }
  
  // optionally can implement something to do when hit
  virtual void onHit() { }
};
