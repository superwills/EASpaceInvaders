#include "TitleScreen.h"

#include "Colors.h"
#include "StdWilUtil.h"
#include "Texture.h"

TitleScreen::TitleScreen( const string &titleText ) {
  
  RectF titleBox = sdl->getWindowRectangle();
  titleBox.size.x *= .75;
  titleBox.size.y *= .33;
  Vector2f titleCenter = sdl->getWindowSize() * Vector2f( .5, .25 );
  titleBox.setCenter( titleCenter );
  titleSprite = Sprite::Text( titleText, titleBox, White ); 
  
  menuItemSize = sdl->getWindowSize() * Vector2f( .25, .25 );
  nextMenuItemPos = titleCenter;
  nextMenuItemPos.y += titleBox.size.y / 2;
  
  // Need the title texture to add anim frames using it
  shared_ptr<Texture> titleTexture = titleSprite->animation.getCurrentFrame().tex;
  
  // create a random color cycle animation
  for( int i = 0; i < 15; i++ ) {
    titleSprite->addAnimationFrame( titleTexture, SDL_RandomSolidColor(), .05 );
  }
  
  Vector2f startMenuItemPos = nextMenuItemPos;
  addMenuItem( "Play" );
  addMenuItem( "Test" );
  
  startMenuItemPos.x -= menuItemSize.x;
  pointer = std::make_shared<Sprite>( RectF( startMenuItemPos, Vector2f( menuItemSize.y ) ), AnimationId::MenuPointer );
}

void TitleScreen::update( float t ) {
  titleSprite->update( t );
  for( auto menuItem : menuItems ) {
    menuItem->update( t );
  }
  pointer->update( t );
}

void TitleScreen::draw() {
  titleSprite->draw();
  for( auto menuItem : menuItems ) {
    menuItem->draw();
  }
  pointer->draw();
}

void TitleScreen::addMenuItem( const string &menuItemText ) {
  
  RectF menuItemBox( nextMenuItemPos, menuItemSize );
  shared_ptr<Sprite> menuItemSprite = Sprite::Text( menuItemText, menuItemBox, White );
  nextMenuItemPos.y += menuItemSize.y;
  
  menuItems.push_back( menuItemSprite );
  
}
