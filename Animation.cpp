#include "Animation.h"

#include "RectF.h"
#include "Texture.h"

Animation::Frame::Frame( SDL_Color iColor, float iDuration ) :
    color( iColor ), duration( iDuration ) { }

Animation::Frame::Frame( shared_ptr<Texture> iTex, SDL_Color iColor, float iDuration ) :
    tex( iTex ), color( iColor ), duration( iDuration ) { }

Animation::Frame::Frame( shared_ptr<Texture> iTex, const RectF &subRectangle, SDL_Color iColor, float iDuration ) :
    tex( iTex ), color( iColor ), duration( iDuration ) {

  subRect = subRectangle.toSDLRect();

}
