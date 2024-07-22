#include "Bunker.h"

#include "Assets.h"
#include "Sprite.h"

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
