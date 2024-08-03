#pragma once

#include "SDLColors.h"
#include "StlUtil.h"

struct Texture;

#include <vector>
#include <optional>
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
    float angle = 0;
    
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
  // These are public so you can edit still the frames
  vector<Frame> frames;
  bool cycles = true; // animation cycles or plays thru once
  
  Animation() {
  }
  
  inline bool isEnded() const {
    return frameIndex >= (int)frames.size() - 1;
  }
  
  void addFrame( const Frame& frame );
  
  const Frame& getCurrentFrame() const;
  
  void update( float t );
};
