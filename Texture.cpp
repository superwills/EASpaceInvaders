#include "Texture.h"

#include "Log.h"
#include "SDLWindow.h"

Texture::Texture( const string &filename, SDL_Renderer *renderer ) : name( filename ) {
  // Load first as a "surface"
  SDL_Surface *surface = IMG_Load( filename.c_str() );
  if( !surface ) {
    error( "Couldn't load surface `%s`!", filename.c_str() );
    return;
  }
  
  loadTextureFromSurface( surface, renderer );
}

Texture::Texture( SDL_Surface *surface, SDL_Renderer *renderer ) {
  name = "Texture from surface";
  loadTextureFromSurface( surface, renderer );
}

Texture::~Texture() {
  if( sdlTex ) {
    SDL_DestroyTexture( sdlTex );
  }
}

bool Texture::isValid() const {
  return (bool)sdlTex;
}

void Texture::loadTextureFromSurface( SDL_Surface *surface, SDL_Renderer *renderer ) {
  // convert the surface to a texture
  sdlTex = SDL_CreateTextureFromSurface( renderer, surface );
  SDL_FreeSurface( surface );
  
  if( !sdlTex ) {
    error( "SDL_CreateTextureFromSurface failed %s", SDL_GetError() );
    return;
  }
  
  // default textures to allow alpha blend.
  SDLWindow::Check( SDL_SetTextureBlendMode( sdlTex, SDL_BLENDMODE_BLEND ), "SDL_SetTextureBlendMode" );
  
  SDL_QueryTexture( sdlTex, 0, 0, &w, &h );
}

