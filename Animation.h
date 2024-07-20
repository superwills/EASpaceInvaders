#pragma once

#include "Colors.h"
#include "StdWilUtil.h"

#include <vector>
using std::vector;

// Really basic animation cycler
struct Animation {
  struct Frame {
    SDL_Texture *tex = 0;
    SDL_Color color = White;
    float duration = 0;
    
    Frame() { }
    Frame( SDL_Texture *iTex, SDL_Color iColor, float iDuration ) :
      tex( iTex ), color( iColor ), duration( iDuration ) { }
  };
  
  Frame ErrFrame; // You get this frame back if you have no animation frames set up.
  
private:
  float time = 0; // Amount of time we've spent on current frame
  int frameIndex = 0;
  vector<Frame> frames;
  
public:  
  Animation() {
  }
  
  // Not in dtor in case you copy this object
  void releaseAllTextures() {
    for( Frame& frame : frames ) {
      if( frame.tex ) {
        SDL_DestroyTexture( frame.tex );
      }
    }
  }
  
  void addFrame( const Frame& frame ) {
    frames.emplace_back( frame );
  }
  
  Frame& getCurrentFrame() {
    if( !isValidIndex( frameIndex, frames ) ) {
      error( "Index %d OOB frames, %zu, set up an animation!", frameIndex, frames.size() );
      return ErrFrame;
    }
    
    return frames[ frameIndex ];
  }
  
  void update( float t ) {
    time += t;
    Frame& frame = getCurrentFrame();
    
    if( time > frame.duration ) {
      cycleArrayIndex( frameIndex, frames.size() );
      time = 0;
    }
  }
};
