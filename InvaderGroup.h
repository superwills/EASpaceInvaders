#pragma once

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

#include "Assets.h"
#include "RectF.h"

class Game;
class Invader;

// Represents a group/wave of invaders.
class InvaderGroup {
  bool invaderReachedBottom = 0;
  
  // Invaders fit in a box this% of the original window size.
  inline static const Vector2f DefaultInvaderBoundsSizePercent = Vector2f( .6, .6 );
  
  inline static Vector2f InvaderSize = Vector2f( 100 ); // dummy val, really computed based on window size
  inline static const Vector2f InterInvaderSpacingPercent = Vector2f( .03, .01 );
  inline static Vector2f InterInvaderSpacing;
  
  inline static const int invadersPerRow = 5; // 11 across in the original game.
  inline static const int DefaultMaxBullets = 2;
  inline static const int DesperationThreshold = 10;
  
  int row = 0; // which row we're on when adding invaders.
  bool movingRight = 1;
  RectF bounds = RectF( 0, 0, 256, 256 );
  
  // Basic AI function. Measure between 0 and 1 of how "desperate" the invader group is.
  // When they get desperate, they move faster, and shoot more bullets
  float aiGetDesperation() const;
  // Update how the group behaves based on how "desperate" they are
  void aiUpdateDesperation();
  void addRow( AnimationId character );
  
public:
  vector< shared_ptr<Invader> > invaders;
  
  inline bool didInvadersWin() { return invaderReachedBottom; }
  inline bool empty() { return invaders.empty(); }
  int getMaxNumBullets() const;
  
  void populate();
  
  void update( float t );
  void draw() const;
  void killAll( bool display );
  void clearDead();
};
