#pragma once

#include "Animation.h"
#include "Colors.h"
#include "RectF.h"
#include "SDLWindow.h"
#include "Vectorf.h"

#include <vector>
using std::vector;

class Game;

class Sprite {
  inline static int NextSpriteId = 0;
  
protected:
  int spriteId = NextSpriteId++;
  string name;    // takes space but helps in debug
  bool hidden = 0;
  bool dead = 0;  // set so that object is removed in cleanup pass, after all objects move.
  
public:
  inline static vector< Sprite* > allSprites;
  static void clearDead();
  
  Animation animation; // Single frame if static.
  
	// This member represents the position & the size of the sprite combined
  RectF box;
	
	Sprite();
  Sprite( const RectF& rectangle );
	Sprite( SDL_Texture* iTex );
  virtual ~Sprite();
  
  void addBlankAnimationFrame();
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
	virtual void die();
  
private:
	void retrieveTexSize();
};

typedef shared_ptr<Sprite> SharedSprite;
