#include "TitleScreen.h"

#include "Colors.h"
#include "StdWilUtil.h"
#include "Texture.h"

TitleScreen::TitleScreen( const string &titleText ) :
    Sprite( RectF( Vector2f( 0, 0 ), sdl->getWindowSize() ) ) {
  
  name = makeString( "Title/`%s`/%s", titleText.c_str(), name.c_str() );
  
  // The texture for this is generated by sdl
  shared_ptr<Texture> tex = sdl->makeTextTexture( titleText, White );
  
  // create a random color cycle animation
  for( int i = 0; i < 255; i++ ) {
    addAnimationFrame( tex, SDL_RandomSolidColor(), .05 );
  }
}
