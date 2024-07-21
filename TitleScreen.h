#pragma once

#include "Sprite.h"

#include <vector>
#include <memory>
using std::shared_ptr, std::vector;

class Sprite;

class TitleScreen {
public:
  shared_ptr<Sprite> titleSprite;
  vector< shared_ptr<Sprite> > menuItems;
  shared_ptr<Sprite> pointer;
  Vector2f menuItemSize, nextMenuItemPos;
  
	TitleScreen( const string &titleText );
  void update( float t );
  void draw(); 
 
  void addMenuItem( const string &menuItemText ); 
};
