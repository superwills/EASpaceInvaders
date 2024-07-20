#include "Controller.h"

#include "Log.h"

Controller::Controller() {
	mouseX = mouseY = 0;
	keystates = SDL_GetKeyboardState( 0 );
}

void Controller::update() {
	SDL_GetRelativeMouseState( &mouseX, &mouseY );
 
  // keep a copy of the prev keystates to be able to tell if a key was just pressed/just released
  //memcpy( prevKeystates, keystates, SDL_NUM_SCANCODES ); // this does NOT work since it's a pointer.
	keystates = SDL_GetKeyboardState( 0 );
}

void Controller::setKeyJustPressed( uint16_t key ) {
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
    return;
  }
  
  keysJustPressed[ key ] = 1;
}

void Controller::setKeyJustReleased( uint16_t key ) {
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
    return;
  }
  
  keysJustReleased[ key ] = 1; 
}

bool Controller::isPressed( uint16_t key ) {
  // The scancode has to be between 0 and SDL_NUM_SCANCODES to be a valid index
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
    return 0;
  }
  return keystates[ key ];
}

bool Controller::justPressed( uint16_t key ) {
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
    return 0;
  }
  return keysJustPressed[ key ];
}

bool Controller::justReleased( uint16_t key ) {
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
    return 0;
  }
  return keysJustReleased[ key ];
}

void Controller::clearEventKeys() {
  memset( keysJustPressed, 0, SDL_NUM_SCANCODES );
  memset( keysJustReleased, 0, SDL_NUM_SCANCODES );
}
