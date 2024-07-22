#include "Controller.h"

#include "Log.h"

Controller::Controller() {
	keystates = SDL_GetKeyboardState( 0 );
}

void Controller::update() {
  // We can poll for the change to the mouse position here.
  SDL_GetMouseState( &xMouse, &yMouse );  //Abs pos of mouse
  SDL_GetRelativeMouseState( &dxMouse, &dyMouse );  //rel changes
   
 
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

void Controller::setMouseJustClicked( uint16_t mouseButton ) {

  mouseButtonsJustPressed[ mouseButton ] = 1;
  
}

void Controller::setKeyJustReleased( uint16_t key ) {
  if( key > SDL_NUM_SCANCODES ) {
    error( "Invalid key index %hu", key );
    return;
  }
  
  keysJustReleased[ key ] = 1; 
}

bool Controller::mouseJustPressed( uint16_t mouseButton ) {
  auto it = mouseButtonsJustPressed.find( mouseButton );
  
  if( it == mouseButtonsJustPressed.end() ) {
    // hasn't been pushed before or doesn't exist
    return 0;
  }
  
  return it->second;
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

bool Controller::justPressedAny( const vector<uint16_t>& keys ) {
  for( uint16_t key : keys ) {
    if( justPressed( key ) ) {
      return 1;
    }
  }
  return 0;
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
  
  // Clear these to off
  for( auto& p : mouseButtonsJustPressed ) {
    p.second = 0;
  }
}
