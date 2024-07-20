#include "Controller.h"

#include "Log.h"

Controller::Controller() {
	mouseX = mouseY = 0;
	keystate = SDL_GetKeyboardState( 0 );
}

void Controller::update() {
	SDL_GetRelativeMouseState( &mouseX, &mouseY );
	keystate = SDL_GetKeyboardState( 0 );
}

bool Controller::isPressed( uint16_t key ) {
  // The scancode has to be between 0 and SDL_NUM_SCANCODES to be a valid index
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
  }
  return keystate[ key ];
}
