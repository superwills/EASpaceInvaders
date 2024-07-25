#pragma once

#include "ICollideable.h"

class QuadtreeNode {
  
};

class Quadtree {
  enum Node { TopLeft, TopRight, BottomRight, BottomLeft };
  QuadtreeNode children[ 4 ];
  
  void add( shared_ptr<ICollideable> obj ) {
  }
};
