#pragma once

#ifdef __APPLE__
#include <SDL2/sdl.h>
#else
#include <sdl.h>
#endif

#include <algorithm>

extern SDL_Color White, Black, Red, Green, Blue, Yellow, Magenta, Cyan,
  Indigo;

inline SDL_Color SDL_ColorMake( Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
	SDL_Color color = { r, g, b, a };
	return color;
}

inline SDL_Color operator*( const SDL_Color& color, float v ) {
  float r = std::clamp( color.r*v, 0.f, 255.f );
  float g = std::clamp( color.g*v, 0.f, 255.f );
  float b = std::clamp( color.b*v, 0.f, 255.f );
  float a = std::clamp( color.a*v, 0.f, 255.f );
  
  return { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
}

SDL_Color SDL_RandomSolidColor();
