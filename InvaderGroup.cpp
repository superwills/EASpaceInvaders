#include "InvaderGroup.h"

#include "Invader.h"
#include <cmath>

void InvaderGroup::addRow( AnimationId character ) {
  

  for( int i = 0; i < invadersPerRow; i++ ) {
    RectF box;
    box.pos.y = row * ( invaderSize.y + interInvaderSpacing.y );
    box.pos.x = i*( invaderSize.x + interInvaderSpacing.x );
    
    box.size = invaderSize;
    
    shared_ptr<Invader> invader = std::make_shared<Invader>( box, character );
    invaders.push_back( invader );
  }
  
  row++; // increment row when done adding the row
}

void InvaderGroup::populate( const RectF &invaderBounds ) {
  
  // Lay the invaders out
  float boardWidth = invaderBounds.size.x;
  interInvaderSpacing = Vector2f( boardWidth ) * Vector2f( .05, .01 );
  
  invaderSize = boardWidth / invadersPerRow;
  
  addRow( AnimationId::E );
  addRow( AnimationId::A );
  addRow( AnimationId::Invader2 );
  addRow( AnimationId::Invader1 );
  addRow( AnimationId::Invader1 );
}

void InvaderGroup::update( float t ) {
  
  if( invaderReachedBottom ) {
    // invaders stop moving once they have won.
    return;
  }
  
  // Step right, until someone hits the edge.
  float invaderVelocity = movingRight ? +DefaultSpeed : -DefaultSpeed;
  
  // multiply velocity increasingly starting when there are 10 invaders left.
  if( invaders.size() < 10 ) {
    float t = invaders.size() / 10.;
    float speedBoost = lerp( 4, 1, t );
    invaderVelocity *= speedBoost;
  }
  
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
      invader->move( 0, invaderSize.y + interInvaderSpacing.y );
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
