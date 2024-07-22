#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

#include <vector>
using std::vector;

// handles user input
class Controller {
  const Uint8 *keystates = 0;
  
  // One-touch key presses are sent by SDL_PollEvent
  Uint8 keysJustPressed[ SDL_NUM_SCANCODES ] = { 0 };
  Uint8 keysJustReleased[ SDL_NUM_SCANCODES ] = { 0 };
  
public:
	int mouseX = 0, mouseY = 0;
  
  Controller();
	void update();
 
  void setKeyJustPressed( uint16_t key );
  void setKeyJustReleased( uint16_t key ); 
  
  bool isPressed( uint16_t key );
  bool justPressed( uint16_t key );
  bool justPressedAny( const vector<uint16_t>& keys );
  bool justReleased( uint16_t key );
  
  void clearEventKeys();
};
