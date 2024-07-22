#pragma once

class Invader;
class Sprite;

#include <memory>
#include <vector>
using std::shared_ptr, std::vector;

#include "Assets.h"

class GameOverScreen {
public:
  shared_ptr<Sprite> gameOverSprite;
  shared_ptr<Sprite> message;
  vector< shared_ptr<Invader> > invaders;
  GameOverScreen();
  
  void addInvaders( AnimationId character );
  void update( float t );
  void draw();
  void win();
  void lose();
  
};
