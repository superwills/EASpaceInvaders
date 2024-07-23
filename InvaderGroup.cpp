#include "InvaderGroup.h"

#include "Invader.h"
#include <cmath>

float InvaderGroup::aiGetDesperation() const {
  float desperation = 0;
  
  // Only start getting desperate when there are 10 invaders or less left.
  if( invaders.size() <= DesperationThreshold ) {
    desperation = ( DesperationThreshold - invaders.size() ) / (float)DesperationThreshold;
    
    // eg Threshold=10, 3 invaders left,
    // (10 - 3) / 10
    // = 7 / 10
    // = .7 desperation.
  }
  
  return desperation;
}

void InvaderGroup::aiUpdateDesperation() {
  // velocity: increases with desperation
  float boost = lerp( 1, 10, aiGetDesperation() );
  Invader::Speed = Invader::DefaultSpeed * boost;
  
  // Chance to shoot: increases with desperation
  Invader::ChanceToShoot = Invader::DefaultChanceToShoot * boost;
}

int InvaderGroup::getMaxNumBullets() const {
  int maxBullets = DefaultMaxBullets;
  
  float bulletMaxBoost = lerp( 1, 10, aiGetDesperation() );  // Use desperation to lerp the param  
  maxBullets *= bulletMaxBoost;
  
  return maxBullets;
}

void InvaderGroup::addRow( AnimationId character ) {
  for( int i = 0; i < invadersPerRow; i++ ) {
    RectF box;
    box.pos = bounds.pos;
    box.pos.y += row * ( invaderSize.y + interInvaderSpacing.y );
    box.pos.x += i*( invaderSize.x + interInvaderSpacing.x );
    
    box.size = invaderSize;
    
    shared_ptr<Invader> invader = std::make_shared<Invader>( box, character );
    invaders.push_back( invader );
  }
  
  row++; // increment row when done adding the row
}

void InvaderGroup::populate( const RectF &invaderBounds ) {
  bounds = invaderBounds;
  // Lay the invaders out
  float boardWidth = invaderBounds.size.x;
  interInvaderSpacing = Vector2f( boardWidth ) * Vector2f( .05, .01 );
  
  invaderSize = boardWidth / invadersPerRow;
  
  addRow( AnimationId::InvaderE );
  addRow( AnimationId::InvaderA );
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
  float invaderSpeed = Invader::Speed;
  
  // Negate speed if not moving right
  if( !movingRight ) {
    invaderSpeed = -invaderSpeed;
  }
  
  aiUpdateDesperation();
  
  Vector2f windowSize = sdl->getWindowSize();
  
  for( auto invader : invaders ) {
    invader->velocity = Vector2f( invaderSpeed, 0 );
    invader->update( t );
    
    // Check lose condition: any invader reaches the bottom of the screen
    if( invader->box.bottom() >= windowSize.y ) {
      invaderReachedBottom = 1; //pick message up outside
      return; // abort processing.
    }
  }
  
  // If an invader would breaks the left/right limits, the direction reverses on the next turn
  bool invaderWillHitSide = 0;
  for( auto invader : invaders ) {
    RectF window = sdl->getWindowRectangle();
    
    RectF next = invader->box + invader->velocity*t;
    
    // Have to check left/right bounds only.
    if( !window.contains( next ) ) {
      invaderWillHitSide = 1;
      break;
    }
  }
  
  if( invaderWillHitSide ) {
    movingRight = !movingRight;
    
    // move down by size of invader.
    Vector2f displacement( 0, invaderSize.y + interInvaderSpacing.y ); 
    for( auto invader : invaders ) {
      invader->move( displacement );
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
  
  Invader::ChanceToShoot = .1;
}

void InvaderGroup::clearDead() {
  clearDeadOnes( invaders );
}
