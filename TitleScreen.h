#pragma once

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

#include "Sprite.h"
#include "GameState.h"

class Sprite;

class TitleScreen {
  shared_ptr<Sprite> titleShadow, titleSprite, pushKey;
  vector< shared_ptr<Sprite> > invaders;
  vector< shared_ptr<Sprite> > menuItems;
  shared_ptr<Sprite> pointer;
  int pointerIndex = 0;
  
  Vector2f startMenuPos, menuItemSize;
  
public:
	TitleScreen( const string &titleText );
  void update( float t );
  void draw();
  
  GameState getLaunchState();
  void addMenuItem( const string &menuItemText );
  
  // Code the formulas for getting these
  float getMenuPointerSize() const;
  float getMenuPointerX() const;
  
  void pointerUp();
  void pointerDown();
  void updatePointerPosition();
};
