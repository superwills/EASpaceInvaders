#pragma once

#include "Sprite.h"

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

class Sprite;

class TitleScreen {
public:
  shared_ptr<Sprite> titleShadow, titleSprite;
  vector< shared_ptr<Sprite> > menuItems;
  shared_ptr<Sprite> pointer;
  int pointerIndex = 0;
  
  Vector2f startMenuPos, menuItemSize;
  
	TitleScreen( const string &titleText );
  void update( float t );
  void draw();
  
  void addMenuItem( const string &menuItemText );
  
  // Code the formulas for getting these
  float getMenuPointerSize() const;
  float getMenuPointerX() const;
  
  void pointerUp();
  void pointerDown();
  void updatePointerPosition();
};
