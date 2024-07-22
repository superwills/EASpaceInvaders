#pragma once

#include "SDLColors.h"
#include "StlUtil.h"

struct Texture;

#include <vector>
using std::shared_ptr, std::vector;

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
    // Frame with no tex, just a solid color.
    Frame( SDL_Color iColor, float iDuration = DefaultFrameTime );
    Frame( shared_ptr<Texture> iTex, SDL_Color iColor, float iDuration = DefaultFrameTime );
    Frame( shared_ptr<Texture> iTex, const RectF &subRectangle, SDL_Color iColor, float iDuration = DefaultFrameTime );
  };
  
  Frame ErrFrame; // You get this frame back if you have no animation frames set up.
  
private:
  float time = 0;  // Amount of time we've spent on current frame
  int frameIndex = 0;
  
public:
  bool cycles = 1; // animation cycles or plays thru once.
  vector<Frame> frames;
  
  Animation() {
  }
  
  inline bool isEnded() const {
    return frameIndex >= (int)frames.size() - 1;
  }
  
  void addFrame( const Frame& frame );
  
  const Frame& getCurrentFrame() const;
  
  void update( float t );
};
