#pragma once

class Invader;
class Sprite;

#include <memory>
#include <vector>
using std::shared_ptr, std::vector;

class GameOverScreen {
public:
  vector< shared_ptr<Sprite> > textSprites;
  vector< shared_ptr<Invader> > invaders;
  GameOverScreen();
  
  void update( float t );
  void draw();
  void win();
  void lose();
  
};
