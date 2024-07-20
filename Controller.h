#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

// handles user input
class Controller {
  const Uint8 *keystate = 0;
  
public:
	int mouseX = 0, mouseY = 0;
  
  Controller();
	void update();
 
  bool isPressed( uint16_t key );
};
