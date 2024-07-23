#pragma once

#include "Vectorf.h"

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

class Sprite;

// Really basic testing needed verify the code was correct
// For point-click testing to make sure intersection routines work correctly
class Test {
  shared_ptr<Sprite> largeSprite;
  vector< shared_ptr<Sprite> > sprites;
  
public:
  Test();
  
  // sprites turn red when hit, yellow if not
  void testMouseHit( const Vector2f &p );
  
  void update( float t );
  void draw();
  
};


