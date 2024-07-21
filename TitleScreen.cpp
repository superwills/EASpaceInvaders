#include "TitleScreen.h"

#include "Colors.h"
#include "StdWilUtil.h"
#include "Texture.h"

TitleScreen::TitleScreen( const string &titleText ) {
  
  shared_ptr<Texture> titleTexture = sdl->makeTextTexture( titleText, White );
  
  titleSprite = std::make_shared<Sprite>( RectF( Vector2f( 0, 0 ), sdl->getWindowSize() ), titleTexture );
  titleSprite->name = makeString( "Title/`%s`/%s", titleText.c_str(), titleSprite->name.c_str() );
  
  // create a random color cycle animation
  for( int i = 0; i < 255; i++ ) {
    titleSprite->addAnimationFrame( titleTexture, SDL_RandomSolidColor(), .05 );
  }
  
  pointer = std::make_shared<Sprite>( RectF( 0, 0, 64, 64 ) );
}

void TitleScreen::update( float t ) {

}

void TitleScreen::draw() {
  titleSprite->draw();
  for( auto menuItem : menuItems ) {
    menuItem->draw();
  }
  pointer->draw();
}

void TitleScreen::addMenuItem( const string &menuItemText ) {
  
  shared_ptr<Texture> menuItemTexture = sdl->makeTextTexture( menuItemText, White );
  
  RectF menuItemPos( 0, 0, 200, 50 );
  shared_ptr<Sprite> sprite = std::make_shared<Sprite>( menuItemPos, menuItemTexture );
  
  
}
