#include <stdio.h>
#include <string>
using namespace std;

#include "SDLWindow.h"
#include "Game.h"
#include "Sprite.h"
#include <time.h>

// procedural aliens?
/*
 E's and A's
 A very big J enemy

 flying saucer's that fly by that say UFO on them.

 8 bit tones

 Default song of:
 https://www.stef.be/bassoontracker

 Particle effects. Can we do a geometry shader?

 GLEW is here:

 /usr/local/Cellar/glew/2.2.0_1/include
*/

int main(int argc, char* args[]) {
  SDLWindow::SDLInit();
	srand( (uint32_t)time( 0 ) );
	
  sdl = new SDLWindow( "Space Invaders", 800, 600 );
  game = new Game;

	while( game->getState() != Game::Exiting ) {
		// Read controller input
		SDL_Event e;

		// SDL_PollEvent returns nonzero while there are more events
		while( SDL_PollEvent( &e ) ) {
			if( e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) {
				// the game will exit on the next frame
				game->setState( Game::Exiting );
			}
			else if( e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_P ) {
				game->togglePause();
			}
			else if( e.type == SDL_KEYDOWN && game->getState() == Game::Title ) {
				game->setState( Game::Running );
			}
		}
		
		game->update();
    game->draw();
	}
  
  delete game;
  delete sdl;
	
	return 0;
}





