#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

extern SDL_Color White, Black, Red, Green, Blue, Yellow, Magenta, Cyan,
  Indigo;

inline SDL_Color SDL_ColorMake( Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
	SDL_Color color = { r, g, b, a };
	return color;
}

inline SDL_Color operator*( const SDL_Color& color, float v ) {
  return { (Uint8)(color.r*v), (Uint8)(color.g*v), (Uint8)(color.b*v), (Uint8)(color.a*v) };
}

SDL_Color SDL_RandomSolidColor();
