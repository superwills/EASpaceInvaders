#include "TitleScreen.h"

#include "Colors.h"
#include "Game.h"
#include "StdWilUtil.h"
#include "Texture.h"

TitleScreen::TitleScreen( const string &titleText ) {
  
  RectF titleBox = sdl->getWindowRectangle();
  titleBox.size *= Vector2f( .75, .33 );
  Vector2f titleCenter = sdl->getWindowSize() * Vector2f( .5, .25 );
  titleBox.setCenter( titleCenter );
  titleSprite = Sprite::Text( titleText, titleBox, White ); 
  
  titleBox.pos += titleBox.size * .01;
  titleShadow = Sprite::Text( titleText, titleBox, White );
  
  // Need the title texture to add anim frames using it
  shared_ptr<Texture> titleTexture = titleSprite->animation.getCurrentFrame().tex;
  
  // create a random color cycle animation
  for( int i = 0; i < 15; i++ ) {
    titleSprite->addAnimationFrame( titleTexture, SDL_RandomSolidColor(), .05 );
  }
  
  // init the menu items
  menuItemSize = sdl->getWindowSize() * Vector2f( .15, .15 );
  startMenuPos = titleCenter;
  startMenuPos.y += titleBox.size.y / 2;
  addMenuItem( "Play" );
  addMenuItem( "Test" );
  
  // Init the pointer
  Vector2f pointerPos = startMenuPos;
  pointerPos.x = getMenuPointerX();
  pointer = std::make_shared<Sprite>( RectF( pointerPos, Vector2f( getMenuPointerSize() ) ), AnimationId::MenuPointer );
}

void TitleScreen::update( float t ) {
  titleShadow->update( t );
  titleSprite->update( t );
  for( auto menuItem : menuItems ) {
    menuItem->update( t );
  }
  pointer->update( t );
}

void TitleScreen::draw() {
  titleShadow->draw();
  titleSprite->draw();
  for( auto menuItem : menuItems ) {
    menuItem->draw();
  }
  pointer->draw();
}

void TitleScreen::hitReturn() {
  if( !pointerIndex ) {
    game->setState( Game::GameState::Running );
  }
  else {
    game->setState( Game::GameState::Test );
  }
}

void TitleScreen::addMenuItem( const string &menuItemText ) {
  
  Vector2f nextMenuItemPos = startMenuPos;
  nextMenuItemPos.y += menuItems.size() * menuItemSize.y;
  
  RectF menuItemBox( nextMenuItemPos, menuItemSize );
  menuItemBox.pos.x -= menuItemSize.x / 2;
  
  shared_ptr<Sprite> menuItemSprite = Sprite::Text( menuItemText, menuItemBox, White );
  menuItems.push_back( menuItemSprite );
}

float TitleScreen::getMenuPointerSize() const {
  // The menu pointer is the same size as the HEIGHT of a menu items
  return menuItemSize.y;
}

float TitleScreen::getMenuPointerX() const {
  // The menu pointer 
  return startMenuPos.x - getMenuPointerSize()/2 - menuItemSize.x;
}

void TitleScreen::pointerUp() {
  decycleArrayIndex( pointerIndex, menuItems.size() );
  updatePointerPosition();
}

void TitleScreen::pointerDown() {
  cycleArrayIndex( pointerIndex, menuItems.size() );
  updatePointerPosition();
}

void TitleScreen::updatePointerPosition() {
  Vector2f pointerPos = startMenuPos;
  pointerPos.x = getMenuPointerX();
  pointerPos.y += menuItemSize.y * pointerIndex;
  
  pointer->box.pos = pointerPos;
}
