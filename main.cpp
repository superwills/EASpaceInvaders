#include <stdio.h>
#include <string>
using namespace std;

#include "SDLWindow.h"
#include "Game.h"
#include "Sprite.h"
#include <time.h>

// coding features (influenced by mojang )
// - Init all vars
// - ptrs are always checked when they can be null
// - asserts, but "assert is not error checking"
// - all array bounds accesses checked, unless "sure"
//   - (the above few may catch quite a few bugs)
// - avoid new/delete and use shared_ptr when possible
// - const as much as possible
// - Exit program at some large failure points (eg renderer won't start)
// - Try to add helpful errors msgs
// - Some comments for any surprising things
// - preferring shared_ptr to raw pointers where possible
// - use auto where type is not ambiguous from line of code
// - encapsulation
// - Most things are done programmatically. I find that easier. Object sizes are found as % of window size.
// - Code tries to be brief while still being clear
// - Errors in the log mean something should be fixed


// TODO:
// Could use events for some things?


int main(int argc, char* args[]) {

  randSeed( (uint32_t)time( 0 ) );
  SDLWindow::SDLInit();
	
  sdl = std::make_shared<SDLWindow>( "Space Invaders", 800, 600 );
  game = std::make_shared<Game>();
  game->init();

	while( !game->isState( GameState::Exiting ) ) {
		// Read controller input
		SDL_Event e;

		// SDL_PollEvent returns nonzero while there are more events
		while( SDL_PollEvent( &e ) ) {
			if( e.type == SDL_QUIT || e.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) {
        game->setState( GameState::Exiting );  // the game will exit on the next frame
			}
      
      // Fork over the signal to the Controller object
      if( e.type == SDL_KEYDOWN ) {
        game->setKeyJustPressed( e.key.keysym.scancode );
      }
      
      if( e.type == SDL_MOUSEBUTTONDOWN ) {
        game->setMouseJustClicked( e.button.button );
      }
		}
		
    game->update();
    game->draw();
	}
  
  game.reset();
  sdl.reset();
	
	return 0;
}





