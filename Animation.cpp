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

void Animation::addFrame( const Frame& frame ) {
  frames.emplace_back( frame );
}

const Animation::Frame& Animation::getCurrentFrame() const {
  if( !isValidIndex( frameIndex, frames ) ) {
    error( "Index %d OOB frames, %zu, set up an animation!", frameIndex, frames.size() );
    return ErrFrame;
  }
  
  return frames[ frameIndex ];
}

void Animation::update( float t ) {
  time += t;
  const Frame& frame = getCurrentFrame();
  
  if( time > frame.duration ) {
    ++frameIndex;
    
    // don't let the index go OOB.
    if( frameIndex >= frames.size() ) {
      if( cycles ) {
        frameIndex = 0;
      }
      else {
        // stay on the last frame.
        frameIndex = (int)frames.size() - 1;
      }
    }
    time = 0;
  }
}
