#pragma once

#include "Colors.h"
#include "StdWilUtil.h"

struct Texture;

#include <vector>
using std::vector;

struct RectF;

// Really basic animation cycler
struct Animation {
  struct Frame {
    shared_ptr<Texture> tex;
    std::optional<SDL_Rect> subRect; // The subportion of the texture to draw. if not set, then draw the whole texture
    SDL_Color color = White;
    float duration = 0;
    inline static float DefaultFrameTime = .1f;
    
    Frame() { }
    Frame( shared_ptr<Texture> iTex, SDL_Color iColor, float iDuration = DefaultFrameTime );
    Frame( shared_ptr<Texture> iTex, const RectF &subRectangle, SDL_Color iColor, float iDuration = DefaultFrameTime );
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
  
  const Frame& getCurrentFrame() const {
    if( !isValidIndex( frameIndex, frames ) ) {
      error( "Index %d OOB frames, %zu, set up an animation!", frameIndex, frames.size() );
      return ErrFrame;
    }
    
    return frames[ frameIndex ];
  }
  
  void update( float t ) {
    time += t;
    const Frame& frame = getCurrentFrame();
    
    if( time > frame.duration ) {
      cycleArrayIndex( frameIndex, frames.size() );
      time = 0;
    }
  }
};
