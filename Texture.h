#pragma once

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <string>
using std::string;

// Light object wrapper around an SDL_Texture to provide destruction
struct Texture {
  string name;
  SDL_Texture *sdlTex = 0;
  int w = 0, h = 0;
  
  Texture( const string &filename, SDL_Renderer *renderer );
  Texture( SDL_Surface *surface, SDL_Renderer *renderer );
  ~Texture();
  void loadTextureFromSurface( SDL_Surface *surface, SDL_Renderer *renderer );
};
