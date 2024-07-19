#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

extern SDL_Color White, Black, Red, Green, Blue, Yellow, Magenta, Cyan;

inline SDL_Color SDL_ColorMake( Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
	SDL_Color color = { r, g, b, a };
	return color;
}

SDL_Color SDL_RandomSolidColor();
