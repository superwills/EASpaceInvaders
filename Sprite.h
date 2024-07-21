#pragma once

#include "Animation.h"
#include "Colors.h"
#include "RectF.h"
#include "SDLWindow.h"
#include "Vectorf.h"

#include <vector>
using std::vector;

class Game;

class Sprite : public std::enable_shared_from_this<Sprite> {
  inline static int NextSpriteId = 0;
  
protected:
  int spriteId = NextSpriteId++;
  string name;    // takes space but helps in debug
  bool hidden = 0;
  
public:
  bool dead = 0;  // set so that object is removed in cleanup pass, after all objects move.
  Animation animation; // Single frame if static.
  
	// This member represents the position & the size of the sprite combined
  RectF box;
	
	Sprite();
  Sprite( const RectF& rectangle );
	Sprite( shared_ptr<Texture> iTex );
  Sprite( const RectF& rectangle, AnimationId animationId ); 
  virtual ~Sprite() { }
  
  void addBlankAnimationFrame();
  void addAnimationFrame( shared_ptr<Texture> tex, SDL_Color color, float duration ); 
  void addAnimationFrame( shared_ptr<Texture> tex, const RectF &subRect, SDL_Color color, float duration );
  
  // Makes a text sprite in the default font
	static shared_ptr<Sprite> Text( const string &text, SDL_Color iColor );
 
  Vector2f getPos();
	Vector2f getCenter();
	void setPos( const Vector2f &pos );
	inline void setPos( float x, float y ) {
		setCenter( Vector2f( x, y ) );
	}
	void setCenter( const Vector2f &pos );
	inline void setCenter( float x, float y ) {
		setCenter( Vector2f( x, y ) );
	}
	void setSize( const Vector2f &size );
	void scale( float s );
	void enforceWorldLimits();
	void show();
	void hide();
  void move( float x, float y );
  
  // usually just the box itself. But when the sprite .pos is the CENTER of the sprite, we have to compute this.
  virtual RectF getDrawBox() const;
  
  // Every sprite can override update / draw, to act/appear differently.
	virtual void update( float t );
	virtual void draw() const;
	virtual void die();
  
};

typedef shared_ptr<Sprite> SharedSprite;
