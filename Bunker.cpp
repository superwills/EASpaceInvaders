#include "Bunker.h"

#include "Assets.h"
#include "Sprite.h"
#include "StlUtil.h"

Bunker::Bunker( const RectF &bounds ) {
  
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
      
      shared_ptr<Sprite> sprite = std::make_shared<Sprite>( bunkerRectangle, Green );
      sprite->name = "BunkerPiece/" + sprite->name;
      pieces.push_back( sprite );
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

bool Bunker::killHit( shared_ptr<Sprite> other ) {

  for( auto bunkerPiece : pieces ) {
    if( bunkerPiece->dead ) {
      // if the piece is dead it's as if it isn't there.
      // Need this check because removal from collections is done only after a frame is complete
      continue;
    }
    
    if( bunkerPiece->hit( other ) ) {
      bunkerPiece->die();
      sdl->playSound( SFXId::ExplodeBunker );
      return 1;
    }
  }
  
  return 0;
}

void Bunker::clearDead() {
  
  clearDeadOnes( pieces );
  
}
