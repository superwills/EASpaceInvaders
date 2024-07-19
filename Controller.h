#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

// handles user input and modifies the game state based on that
class Controller
{
public:
	int mouseX, mouseY;
	const Uint8 *keystate;
	Controller();
	// Updates the controller -- call once per frame
	void update();
};
