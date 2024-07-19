#include "TitleScreen.h"
#include "Colors.h"

TitleScreen::TitleScreen( const string &titleText ) :
    Sprite( RectF( 0, 0, sdl->winWidth, sdl->winHeight ) ) {
}

void TitleScreen::update() {
	
  // Use a random color
  //SDL_SetTextureColorMod( tex, color.r, color.g, color.b );
}
