#pragma once

class Invader;
class Sprite;

#include <memory>
#include <vector>
using std::shared_ptr, std::vector;

#include "Assets.h"

class GameOverScreen {
  shared_ptr<Sprite> gameOverSprite;
  shared_ptr<Sprite> message;
  vector< shared_ptr<Invader> > invaders;

public:
  GameOverScreen();  
  void addInvaders( AnimationId character1, AnimationId character2 );
  void update( float t );
  void draw();
  void win();
  void lose();
  
};
