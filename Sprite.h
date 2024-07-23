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
  Vector2f hitBoxScale = 1;   // < 1 for more forgiving collision checks
  // This member represents the position & the size of the sprite combined
  RectF box;
  Vector2f velocity;
	
public:
  string name;    // helps in debug
  bool dead = 0;  // set so that object is removed in cleanup pass, after all objects move.
  Animation animation;  // Single frame for non-animated objects
  SFXId deathSound = SFXId::NoSFX;
  
  // When object dies, # particles it displays
  int minParticles = 8, maxParticles = 12;
  float particleSizeMin = 4, particleSizeMax = 12;
  
  // also tells what kind of thing object is (so you can get the score for it)
  AnimationId character = AnimationId::NoAnimation;
  
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
  
  RectF getScaledHitBox() const;
  
  inline Vector2f getPos() const {  return box.pos;  }
  
  void setPos( const Vector2f &pos ) {  box.pos = pos;  }
  // Set position of sprite so it's center is @pos (accounting for current size of sprite) 
  void setCenter( const Vector2f &pos ) { box.setCenter( pos ); }
  Vector2f getCenter() {  return box.centroid();  }
  void setSize( const Vector2f &size ) {  box.size = size;  }
  
  void enforceWorldLimits();
	void move( const Vector2f &displacement );
  
  bool hit( const Vector2f &p );
  bool hit( const RectF &rect );
  bool hit( shared_ptr<Sprite> other );
  
  // Every sprite can override update / draw, to act/appear differently.
	virtual void update( float t );
	virtual void draw() const;
	virtual void die();
 
  int getScore() const; 
  
};

typedef shared_ptr<Sprite> SharedSprite;
