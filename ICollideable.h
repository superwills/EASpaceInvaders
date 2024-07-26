#pragma once

#include "Vectorf.h"
#include "RectF.h"

#include "StlUtil.h"

class ICollideable {
public:
  virtual RectF getHitBox() const = 0;
  
  // For placement in the Quadtree
  virtual Vector2f getCentroid() const = 0;
  
  virtual string getName() const = 0;
  
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

DECLARE_SHARED_PTR( ICollideable );
