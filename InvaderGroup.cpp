#include "InvaderGroup.h"

#include "Invader.h"

void InvaderGroup::populate( const RectF &invaderBounds ) {
  // Lay the invaders out
  float boardSize = invaderBounds.w;
  invaderSize = boardSize / invadersPerRow;
  
  for( int row = 0; row < rowsOfInvaders; row++ ) {
    for( int i = 0; i < invadersPerRow; i++ ) {
      RectF box;
      box.y = (rowsOfInvaders - row) * ( invaderSize + 5 );
      box.x = i*( invaderSize + 5 );
      
      box.w = box.h = invaderSize;
      
      shared_ptr<Invader> invader = std::make_shared<Invader>( box, rand<AnimationId>( AnimationId::A, AnimationId::E ) );
      invaders.push_back( invader );
    }
  }
}

void InvaderGroup::update() {
  // Step right, until someone hits the edge.
  float disp = movingRight ? +1 : -1;
  
  for( auto invader : invaders ) {
    invader->move( disp, 0 );
    invader->update();
  }
  
  // If an invader would breaks the left/right limits, the direction reverses on the next turn
  bool invaderHitSide = 0;
  for( auto invader : invaders ) {
    RectF window( Vector2f( 0 ), sdl->getWindowSize() );
    
    RectF next = invader->box + disp;
    if( !window.contains( next ) ) {
      invaderHitSide = 1;
      break;
    }
  }
  
  if( invaderHitSide ) {
    movingRight = !movingRight;
    // move down by size of invader.
    for( auto invader : invaders ) {
      invader->move( 0, invaderSize );
    }
  }
}

void InvaderGroup::draw() const {
  for( auto invader : invaders ) {
    invader->draw();
  }
}

void InvaderGroup::clearDead() {
  invaders.erase( std::remove_if( invaders.begin(), invaders.end(), []( shared_ptr<Invader> invader ) {
    return invader->dead;
  } ), invaders.end() );
}
