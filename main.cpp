#include <stdio.h>
#include <string>
using namespace std;

#include "SDLWindow.h"
#include "Game.h"
#include "Sprite.h"
#include <time.h>

// Coding features.
// I'm influenced by the Mojang standard from working on Minecraft
// - Init all vars
// - Array bounds access & ptrs are always checked when they can be null
//   - May catch ~90%? of bugs
// - Exit program at some large failure points (eg renderer won't start)
// - Code tries to be brief while still being clear
// - Try to add helpful errors msgs
// - Some comments for any surprising things



// Powerups? Laser? Spread gun?

// procedural aliens?
/*
 A very big J enemy

 flying saucer's that fly by that say UFO on them.

 Particle effects. Can we do a geometry shader?

 GLEW is here:

 /usr/local/Cellar/glew/2.2.0_1/include
*/

int main(int argc, char* args[]) {
  SDLWindow::SDLInit();
	srand( (uint32_t)time( 0 ) );
	
  sdl = new SDLWindow( "Space Invaders", 800, 600 );
  game = new Game;
  game->init();

	while( !game->isState( Game::GameState::Exiting ) ) {
		// Read controller input
		SDL_Event e;

		// SDL_PollEvent returns nonzero while there are more events
		while( SDL_PollEvent( &e ) ) {
			if( e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) {
        game->setState( Game::GameState::Exiting );  // the game will exit on the next frame
			}
      else if( e.type == SDL_KEYDOWN ) {
        game->controller.setKeyJustPressed( e.key.keysym.scancode );
        
        if( game->isState( Game::GameState::Title ) ) {
          // any key down at title starts the game.
          game->setState( Game::GameState::Running );
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





