#pragma once

#include "Vectorf.h"
#include "RectF.h"

#include "StlUtil.h"

// What kind of object is this, for collision purposes?
enum class ICollideableType {
  NotCollideable,
  
  Bullet,
  Bunker,
  BunkerPiece,
  Invader,
  Item,
  Player,
  UFO,
};

class ICollideable {
public:
  // Each object is required to tell us what kind of object it is.
  // That way, the collision interface stays simple and can be applied
  // to things other than Sprites.
  // We can use the type information to downcast without resorting to dynamic_casts to check
  virtual ICollideableType getType() const { return ICollideableType::NotCollideable; }
    
  virtual RectF getHitBox() const = 0;
  
  // For placement in the Quadtree
  virtual Vector2f getCentroid() const = 0;
  
  virtual string getName() const = 0;
  
  virtual bool hit( const Vector2f &pt ) {
    return getHitBox().hit( pt );
  }
  virtual bool hit( const RectF &rect ) {
    return getHitBox().hit( rect );
  }
  virtual bool hit( shared_ptr<ICollideable> other ) {
    bool didHit = getHitBox().hit( other->getHitBox() );
    if( didHit ) {
      onHit( other );
    }
    return didHit;
  }
  
  // optionally can implement something to do when hit
  // usually objects should manipulate themselves and only query o for information
  // (rather than also telling o what to do when hit)
  virtual void onHit( shared_ptr<ICollideable> o ) { }
};

DECLARE_SHARED_PTR( ICollideable );
