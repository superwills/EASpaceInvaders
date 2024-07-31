#pragma once

#include "Vectorf.h"
#include "RectF.h"

#include "StlUtil.h"
#include <assert.h>

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
  TestSprite,
};

class ICollideable {
protected:
  // Things that can be hit also have a concept of being dead (no longer hittable/responsive to hits)
  bool dead = 0;  // When dead, object is removed in cleanup pass, after all objects move.
  // Setting something to dead should only be done by a die() call
  
public:
  // Using this tag, collision interface stays simple
  // Can use this information to reliably downcast to concrete types
  ICollideableType collisionType = ICollideableType::NotCollideable;
  
  inline bool isDead() const { return dead; }
  
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
  
  // Tests if ICollideables hit each other without triggering onHit events
  virtual bool testHit( shared_ptr<ICollideable> o ) {
    return getHitBox().hit( o->getHitBox() );
  }
  
  virtual bool hit( shared_ptr<ICollideable> o ) {
    // Hard assert can be annoying
    //assert( collisionType != ICollideableType::NotCollideable &&
    //        o->collisionType != ICollideableType::NotCollideable && "evaluating collision of uncollideables" );
    // Either forgot to set collision type, or you're computing collision on uncollideables
    if( collisionType == ICollideableType::NotCollideable || o->collisionType == ICollideableType::NotCollideable ) {
      error( "evaluating collision of uncollideables" );
    }
    
    if( isDead() || o->isDead() ) {
      error( "Collision checking dead objects: %s [%s] vs %s [%s]",
        getName().c_str(), isDead()?"dead":"alive",
        o->getName().c_str(), o->isDead()?"dead":"alive" );
      return false;
    }
    
    bool didHit = getHitBox().hit( o->getHitBox() );
    if( didHit ) {
      // Trigger hit to BOTH objects.
      onHit( o.get() );
      o->onHit( this );
    }
    return didHit;
  }
  
  // This function takes a plain pointer because : enable_shared_from_this<ICollideable> collides with Sprite
  // optionally can implement something to do when hit
  // usually objects should manipulate themselves and only query o for information
  // (rather than also telling o what to do when hit)
  virtual void onHit( ICollideable *o ) { }
  
  virtual void die() { }
};

DECLARE_SHARED_PTR( ICollideable );
