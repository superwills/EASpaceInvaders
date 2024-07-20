#pragma once

#include "SDLWindow.h"
#include "Vectorf.h"
#include "RectF.h"
#include "Colors.h"

#include <vector>
using std::vector;

class Game;

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
  inline static Frame BlankFrame;
  
private:
  float time = 0; // Amount of time we've spent on current frame
  int cf = 0;
  vector<Frame> frames;
  
public:  
  Animation() {
  }
  
  void addFrame( const Frame& frame ) {
    frames.emplace_back( frame );
  }
  
  Frame& getCurrentFrame() {
    // If the animation is empty when you try to grab from it, it creates a blank frame that you get back
    if( frames.empty() ) {
      frames.emplace_back( BlankFrame );
    }
    
    // Ensure the index is in a valid range. frames.size() >= 1 here.
    ::clamp( cf, 0, (int)frames.size() );
    return frames[ cf ];
  }
  
  void update( float t ) {
    if( frames.empty() ) {
      // No animation
      return;
    }
    
    time += t;
    Frame& f = getCurrentFrame();
    
    if( time > f.duration ) {
      cycleFlag( cf, 0, (int)frames.size() );
      time = 0;
    }
  }
};

class Sprite {
  inline static int NextSpriteId = 0;
  
protected:
  int spriteId = NextSpriteId++;
  string name;    // every object has a name for debug
  bool hidden = 0;

public:
  Animation animation; // Single frame if static.
  
	// This member represents the position & the size of the sprite combined
  RectF box;
	
	Sprite();
  Sprite( const RectF& rectangle );
	Sprite( SDL_Texture* iTex );
 
  void addAnimationFrame( SDL_Texture *tex, SDL_Color color, float duration ); 
	
  // Makes a text sprite in the default font
	static Sprite* Text( const string &text, SDL_Color iColor );
	Vector2f getPos();
	Vector2f getCenter();
	void setPos( const Vector2f &pos );
	void setPos( float x, float y ) {
		setCenter( Vector2f( x, y ) );
	}
	void setCenter( Vector2f pos );
	void setCenter( float x, float y ) {
		setCenter( Vector2f( x, y ) );
	}
	void setSize( const Vector2f &size );
	void scale( float s );
	void enforceWorldLimits();
	void show();
	void hide();
  
  // Every sprite can override update / draw, to act/appear differently.
	virtual void update();
	virtual void draw();
	
private:
	void retrieveTexSize();
};

