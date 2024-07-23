#pragma once

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

#include "Assets.h"
#include "RectF.h"

class Invader;

// Represents a group/wave of invaders.
class InvaderGroup {
  bool invaderReachedBottom = 0;
  
public:
  inline static Vector2f invaderSize = Vector2f( 100 ); // dummy val, really computed based on window size
  inline static Vector2f interInvaderSpacing = Vector2f( 5, 2 ); // dummy val.
  
  inline static const int rowsOfInvaders = 5;
  inline static const int invadersPerRow = 11; // 11 across in the original game.
  
  // Default speed before speedup due to fewer invaders left
  inline static const float DefaultSpeed = 25;
  inline static const int DefaultMaxBullets = 2;
  
  int row = 0; // which row we're on when adding invaders.
  
  bool movingRight = 1;
  
  RectF bounds = RectF( 0, 0, 256, 256 );
  
  vector< shared_ptr<Invader> > invaders;
  
  inline bool didInvadersWin() { return invaderReachedBottom; }
  inline bool empty() { return invaders.empty(); }
  int getMaxNumBullets() const;
  
  void addRow( AnimationId character );
  void populate( const RectF &invaderBounds );
  
  void update( float t );
  void draw() const;
  void killAll( bool display );
  void clearDead();
};
