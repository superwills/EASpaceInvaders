#include "Bunker.h"

#include "Assets.h"
#include "Bullet.h"
#include "BunkerPiece.h"
#include "Sprite.h"
#include "StlUtil.h"

Bunker::Bunker( const RectF &initialBounds ) :
    bounds( initialBounds ) {
  
  collisionType = ICollideableType::Bunker;
  Vector2f bunkerPieceSize = bounds.size / 3;
  
  // 3 across, 3 down (skip the middle bottom one)
  for( int i = 0; i < 3; i++ ) {
    for( int j = 0; j < 3; j++ ) {
      if( i == 1 && j == 2 ) {
        continue; // skip the middle bottom entry
      }
      
      Vector2f bunkerPiecePos = bounds.topLeft();
      // Offset by piece #
      bunkerPiecePos.x += i * bunkerPieceSize.x;
      bunkerPiecePos.y += j * bunkerPieceSize.y;
      RectF bunkerRectangle( bunkerPiecePos, bunkerPieceSize );
      
      BunkerPiece bp(bunkerRectangle, Green );
      auto bunkerPiece = std::make_shared<BunkerPiece>( bunkerRectangle, Green );
      pieces.push_back( bunkerPiece );
    }
  }
}

void Bunker::update( float t ) {
  for( auto bunkerPiece : pieces ) {
    bunkerPiece->update( t );
  }
}

void Bunker::draw() {
  for( auto bunkerPiece : pieces ) {
    bunkerPiece->draw();
  }
}

bool Bunker::hit( shared_ptr<ICollideable> o ) {
  
  // hitting the bunker just means you hit it's bounds.
  // you have to do a 2nd check to see if you hit any of it's pieces or not.
  
  // don't bother to check any pieces if sprite misses the whole bunker bounds
  return o->hit( bounds );
}

bool Bunker::checkPiecesHit( shared_ptr<ICollideable> o ) {
  
  vector<SP_BunkerPiece> hits;

  for( auto bunkerPiece : pieces ) {
    if( bunkerPiece->isDead() ) {
      // if the piece is dead it's as if it isn't there.
      // (check b/c removal from collections only happens after a frame is complete)
      continue;
    }
    
    // testHit so you don't trigger onHit yet (only for closest one)
    if( bunkerPiece->testHit( o ) ) {
      hits.push_back( bunkerPiece );
    }
  }
  
  SP_BunkerPiece closestBunkerPiece;
  float closestDist2 = HUGE_VALF;
  // Give you the closest one to centroid
  for( auto bunkerPiece : hits ) {
    float dist2 = ( o->getCentroid() - bunkerPiece->getCentroid() ).len2();
    
    if( dist2 < closestDist2 ) {
      closestDist2 = dist2;
      closestBunkerPiece = bunkerPiece;
    }
  }
  
  if( closestBunkerPiece ) {
    // Now trigger onHit events for closest only
    closestBunkerPiece->onHit( o.get() );
    o->onHit( closestBunkerPiece.get() );
    return 1;
  }
  else {
    return 0;
  }
}

void Bunker::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet: {
      auto bullet = ((Bullet*)o)->shared_Bullet();
      checkPiecesHit( bullet );
    }
    break;
  case ICollideableType::Bunker:
    break;
  case ICollideableType::BunkerPiece:
    break;
  case ICollideableType::Invader:
    break;
  case ICollideableType::Item:
    break;
  case ICollideableType::Player:
    break;
  case ICollideableType::UFO:
    break;
    
  case ICollideableType::NotCollideable:
  default:
    error( "Colliding with non-collideable" );
    break;
  }
}

void Bunker::clearDead() {
  
  clearDeadOnes( pieces );
  
}
