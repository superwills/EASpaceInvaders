#include "TitleScreen.h"

#include "Colors.h"
#include "StdWilUtil.h"
#include "Texture.h"

TitleScreen::TitleScreen( const string &titleText ) {
  
  titleSprite = Sprite::Text( titleText, sdl->getWindowRectangle(), White ); 
  // Need the title texture to add anim frames using it
  shared_ptr<Texture> titleTexture = titleSprite->animation.getCurrentFrame().tex;
  
  // create a random color cycle animation
  for( int i = 0; i < 255; i++ ) {
    titleSprite->addAnimationFrame( titleTexture, SDL_RandomSolidColor(), .05 );
  }
  
  pointer = std::make_shared<Sprite>( RectF( 0, 0, 12, 12 ) );
  pointer->addAnimationFrame( 0, White, 1 );
  
  addMenuItem( "Play" );
  addMenuItem( "Test" );
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
