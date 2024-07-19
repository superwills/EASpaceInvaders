#include <stdio.h>
#include <string>
using namespace std;

#include "SDLWindow.h"
#include "Game.h"
#include "Sprite.h"
#include <time.h>

int main(int argc, char* args[]) {
  SDLWindow::SDLInit();
	srand( time( 0 ) );
	
  SDLWindow sdl( "Ping", 800, 600 );
	
	Game game( sdl );

	while( game.getState() != Game::Exiting ) {
		// Read controller input
		SDL_Event e;

		// SDL_PollEvent returns nonzero while there are more events
		while( SDL_PollEvent( &e ) ) {
			if( e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) {
				// the game will exit on the next frame
				game.setState( Game::Exiting );
			}
			else if( e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_P ) {
				game.togglePause();
			}
			else if( e.type == SDL_KEYDOWN && game.getState() == Game::Title ) {
				game.setState( Game::Running );
			}
		}
		
		game.update();
    game.draw();
	}
	
	return 0;
}





