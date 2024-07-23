#pragma once

#include "Animation.h"
#include "SDLColors.h"
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
  bool hidden = 0;
  
public:
  string name;    // takes space but helps in debug
  bool dead = 0;  // set so that object is removed in cleanup pass, after all objects move.
  Animation animation; // Single frame if static.
  SFXId deathSound = SFXId::NoSFX;
  
  // When object dies, # particles it displays
  int minParticles = 8, maxParticles = 12;
  float particleSizeMin = 4, particleSizeMax = 12;
  
  // Besides the class type, this tells you precisely what kind of thing this is (so you can get the score for it)
  AnimationId character = AnimationId::NoAnimation;
  
	// This member represents the position & the size of the sprite combined
  RectF box;
  
  // Moves the sprite each frame.
  Vector2f velocity;
	
	Sprite();
  Sprite( const RectF &rectangle ); 
  Sprite( const RectF &rectangle, SDL_Color color );
	Sprite( const RectF &rectangle, shared_ptr<Texture> iTex, SDL_Color color );
  Sprite( const RectF &rectangle, AnimationId animationId ); 
  virtual ~Sprite() { }
  
  // add an animation frame where the object is a solid color
  void addSolidColorAnimationFrame( SDL_Color color );
  void addAnimationFrame( shared_ptr<Texture> tex, SDL_Color color ); 
  void addAnimationFrame( shared_ptr<Texture> tex, SDL_Color color, float duration ); 
  void addAnimationFrame( shared_ptr<Texture> tex, const RectF &subRect, SDL_Color color, float duration );
  
  // Makes a text sprite in the default font
	static shared_ptr<Sprite> Text( const string &text, const RectF &box, SDL_Color iColor );
 
  Vector2f getPos();
	Vector2f getCenter();
	void setPos( const Vector2f &pos );
  // Set position of sprite so it's center is @pos (accounting for current size of sprite) 
  void setCenter( const Vector2f &pos ); 
	void setSize( const Vector2f &size );
	void scale( float s );
	void enforceWorldLimits();
	void show();
	void hide();
  void move( float x, float y );
  
  bool hit( const Vector2f &p );
  bool hit( shared_ptr<Sprite> other );
  
  // usually just the box itself. But when the sprite .pos is the CENTER of the sprite, we have to compute this.
  virtual RectF getDrawBox() const;
  
  // Every sprite can override update / draw, to act/appear differently.
	virtual void update( float t );
	virtual void draw() const;
	virtual void die();
 
  int getScore() const; 
  
};

typedef shared_ptr<Sprite> SharedSprite;
