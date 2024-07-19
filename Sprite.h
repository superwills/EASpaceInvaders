#pragma once

#include "SDLWindow.h"
#include "Vectorf.h"
#include "RectF.h"
#include "Colors.h"

class Game;

class Sprite {
  inline static int NextSpriteId = 0;
  
protected:
  int spriteId = NextSpriteId++;
  string name;    // every object has a name for debug
  bool hidden = 0;

public:
  SDL_Color color = White;
  SDL_Texture *tex = 0; // Texture, if any
  
	// This member represents the position & the size of the sprite combined
  RectF box;
	
	Vector2f vel;
	
	Sprite();
	Sprite( SDL_Texture* iTex );
	Sprite( string filename );
  
  // Makes a text sprite in the default font
	static Sprite* Text( string text, SDL_Color iColor );
	Vector2f getPos();
	Vector2f getCenter();
	void setPos( Vector2f pos );
	void setPos( float x, float y ) {
		setCenter(Vector2f(x,y));
	}
	void setCenter( Vector2f pos );
	void setCenter( float x, float y ) {
		setCenter(Vector2f(x,y));
	}
	void setSize( Vector2f size );
	void scale( float s );
	void bounceTopAndBottom();
	void bounceLeftAndRight();
	void show();
	void hide();
  
  // Every sprite can override update / draw, to act/appear differently.
	virtual void update();
	virtual void draw();
	
private:
	void retrieveTexSize();
};

