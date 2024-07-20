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
      
    void println() {
      printf( "%d %d %f\n", tex, color, duration );
    }
  };
  
  Frame ErrFrame; // You get this frame back if you have no animation frames set up.
  
private:
  float time = 0; // Amount of time we've spent on current frame
  int frameIndex = 0;
  vector<Frame> frames;
  
public:  
  Animation() {
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
    if( frames.empty() ) {
      // No animation
      return;
    }
    
    time += t;
    Frame& f = getCurrentFrame();
    
    if( time > f.duration ) {
      cycleArrayIndex( frameIndex, frames.size() );
      time = 0;
    }
  }
};
