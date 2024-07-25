#include "Bunker.h"

#include "Assets.h"
#include "BunkerPiece.h"
#include "Sprite.h"
#include "StlUtil.h"

Bunker::Bunker( const RectF &initialBounds ) :
    bounds( initialBounds ) {
  
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

RectF Bunker::getHitBox() const {
  return bounds;
}

bool Bunker::killHit( shared_ptr<Sprite> other ) {
  
  // don't bother to check any pieces if sprite misses the whole bunker bounds
  if( !other->hit( bounds ) ) {
    return 0;
  }
  
  for( auto bunkerPiece : pieces ) {
    if( bunkerPiece->isDead() ) {
      // if the piece is dead it's as if it isn't there.
      // (check b/c removal from collections only happens after a frame is complete)
      continue;
    }
    
    if( bunkerPiece->hit( other ) ) {
      bunkerPiece->die();
      return 1;
    }
  }
  
  return 0;
}

void Bunker::clearDead() {
  
  clearDeadOnes( pieces );
  
}
