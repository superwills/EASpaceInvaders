#include "InvaderGroup.h"

#include "Invader.h"

void InvaderGroup::addRow( AnimationId character ) {
  for( int i = 0; i < invadersPerRow; i++ ) {
    RectF box;
    box.pos.y = row * ( invaderSize + 5 );
    box.pos.x = i*( invaderSize + 5 );
    
    box.size = invaderSize;
    
    shared_ptr<Invader> invader = std::make_shared<Invader>( box, character );
    invaders.push_back( invader );
  }
  
  row++; // increment row when done adding the row
}

void InvaderGroup::populate( const RectF &invaderBounds ) {
  // Lay the invaders out
  float boardSize = invaderBounds.size.x;
  invaderSize = boardSize / invadersPerRow;
  
  addRow( AnimationId::E );
  addRow( AnimationId::A );
  //addRow( AnimationId::Invader2 );
  //addRow( AnimationId::Invader2 );
  
  addRow( AnimationId::Invader2 );
  addRow( AnimationId::Invader1 );
  addRow( AnimationId::Invader1 );
  
  /*
  for( int row = 0; row < rowsOfInvaders; row++ ) {
    for( int i = 0; i < invadersPerRow; i++ ) {
      RectF box;
      box.y = (rowsOfInvaders - row) * ( invaderSize + 5 );
      box.x = i*( invaderSize + 5 );
      
      box.w = box.h = invaderSize;
      
      shared_ptr<Invader> invader = std::make_shared<Invader>( box, rand<AnimationId>( AnimationId::Invader1, AnimationId::E ) );
      invaders.push_back( invader );
    }
  }
  */
}

void InvaderGroup::update( float t ) {
  
  if( invaderReachedBottom ) {
    // invaders stop moving once they have won.
    return;
  }
  
  // Step right, until someone hits the edge.
  float invaderVelocity = movingRight ? +100 : -100;
  Vector2f windowSize = sdl->getWindowSize();
  
  for( auto invader : invaders ) {
    invader->velocity = Vector2f( invaderVelocity, 0 );
    invader->update( t );
    
    // Check lose condition: any invader reaches the bottom of the screen
    if( invader->box.bottom() >= windowSize.y ) {
      invaderReachedBottom = 1; //pick message up outside
      return; // abort processing.
    }
  }
  
  // If an invader would breaks the left/right limits, the direction reverses on the next turn
  bool invaderHitSide = 0;
  for( auto invader : invaders ) {
    RectF window( Vector2f( 0 ), windowSize );
    
    RectF next = invader->box + invader->velocity*t;
    
    // Have to check left/right bounds only.
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

void InvaderGroup::killAll( bool display ) {
  if( display ) {
    for( auto invader : invaders ) {
      invader->die();
    }
  }
  invaders.clear();
  
  invaderReachedBottom = 0;
  row = 0;
}

void InvaderGroup::clearDead() {
  clearDeadOnes( invaders );
}
