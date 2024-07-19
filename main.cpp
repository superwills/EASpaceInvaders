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
        game->setState( Game::Exiting );  // the game will exit on the next frame
			}
      else if( e.type == SDL_KEYDOWN ) {
        if( game->isState( Game::Title ) ) {
          // any key down at title starts the game.
          game->setState( Game::Running );
        }
        else {
          // p during play pauses the game.
          if( e.key.keysym.scancode == SDL_SCANCODE_P ) {
            game->togglePause();
          }
        }
      }
		}
		
		game->update();
    game->draw();
	}
  
  delete game;
  delete sdl;
	
	return 0;
}





