#include "SDLColors.h"

#include "randomUtil.h"

SDL_Color White = { 255, 255, 255, 255 },
  Black = { 0, 0, 0, 255 },
  Red = { 255, 0, 0, 255 },
  Green = { 0, 255, 0, 255 },
  Blue = { 0, 0, 255, 255 },
  Yellow = { 255, 255, 0, 255 },
  Magenta = { 255, 0, 255, 255 },
  Cyan = { 0, 255, 255, 255 },
  Indigo = { 0, 0, 40, 255 };

SDL_Color SDL_RandomSolidColor() {
  return SDL_ColorMake( randInt( 256 ), randInt( 256 ), randInt( 256 ), 255 );
}
