#pragma once

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

class Invader;

// Represents a group/wave of invaders.
class InvaderGroup {
public:
  vector< shared_ptr<Invader> > invaders;
  void move();
  void draw() const;
  void clearDead();
};
