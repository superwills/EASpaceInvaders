#include "Animation.h"

#include "Texture.h"

Animation::Frame::Frame( shared_ptr<Texture> iTex, SDL_Color iColor, float iDuration ) :
    tex( iTex ), color( iColor ), duration( iDuration ) { }
