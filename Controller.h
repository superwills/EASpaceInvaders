#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

#include "Vectorf.h"

#include <map>
#include <vector>
using std::map, std::vector;

// handles user input
class Controller {
  const Uint8 *keystates = 0;
  
  // One-touch key presses are sent by SDL_PollEvent
  Uint8 keysJustPressed[ SDL_NUM_SCANCODES ] = { 0 };
  Uint8 keysJustReleased[ SDL_NUM_SCANCODES ] = { 0 };
  map< uint16_t, bool > mouseButtonsJustPressed;
  
  int xMouse = 0, yMouse = 0;   //abs mouse x,y
	int dxMouse = 0, dyMouse = 0; //Difference mouse x,y
  
public:
  Controller();
	void update();
 
  inline float getMouseDX() { return dxMouse; }
  inline float getMouseDY() { return dyMouse; }
  inline Vector2f getMousePos() { return Vector2f( xMouse, yMouse ); } 
 
  void setKeyJustPressed( uint16_t key );
  void setMouseJustClicked( uint16_t mouseButton );
  
  void setKeyJustReleased( uint16_t key ); 
  
  bool mouseJustPressed( uint16_t mouseButton );
  bool isPressed( uint16_t key );
  bool justPressed( uint16_t key );
  bool justPressedAny( const vector<uint16_t>& keys );
  bool justReleased( uint16_t key );
  
  void clearEventKeys();
};
