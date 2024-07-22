#pragma once

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

class Sprite;

// Functional tests with tester interaction to verify
// For point-click testing to make sure intersection routines work correctly
class Test {
  
  vector< shared_ptr<Sprite> > sprites;
  
  void unitRandom();
  void fnTestShooting();
  
  void fnHitTest();
  
};


