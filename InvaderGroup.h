#pragma once

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

#include "RectF.h"

class Invader;

// Represents a group/wave of invaders.
class InvaderGroup {
public:
  const int rowsOfInvaders = 5;
  const int invadersPerRow = 11; // 11 across in the original game.
  bool movingRight = 1;
  float invaderSize = 64; 
  
  RectF bounds = RectF( 0, 0, 256, 256 );
  
  vector< shared_ptr<Invader> > invaders;
  void populate( const RectF &invaderBounds );
  void update();
  void draw() const;
  void clearDead();
};
