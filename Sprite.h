#pragma once

#include "Animation.h"
#include "ICollideable.h"
#include "ParticleCloudProperties.h"
#include "SDLColors.h"
#include "RectF.h"
#include "SDLWindow.h"
#include "Vectorf.h"

#include <vector>
using std::vector;

class Game;

class Sprite : public ICollideable, public std::enable_shared_from_this<Sprite> {
  inline static int NextSpriteId = 0;
  
protected:
  float hp = 1;
  int spriteId = NextSpriteId++;
  Vector2f hitBoxScale = 1;   // < 1 for more forgiving collision checks
  // This member represents the position & the size of the sprite combined
  RectF box;
  AnimationId itemDrop = AnimationId::NoAnimation;
 
  // Child sprites that move with/are drawn with this sprite.
  vector< shared_ptr<Sprite> > children;
  bool dieOnAnimationEnd = 0;
  bool dead = 0;  // set so that object is removed in cleanup pass, after all objects move.
  
public:
  string name;    // for tracing in debug
  
  Animation animation;  // Single frame for non-animated objects
  Vector2f velocity;
  
  // also tells what kind of thing object is (so you can get the score for it)
  AnimationId character = AnimationId::NoAnimation;
  AnimationId deathAnimation = AnimationId::NoAnimation; // replace self with death animation when die
  SFXId deathSound = SFXId::NoSFX;
  
  // When object dies, # particles it displays
  ParticleCloudProperties particleCloudProperties;
  
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
  void setAnimation( AnimationId animationId );
  
  // Makes a text sprite in the default font
	static shared_ptr<Sprite> Text( const string &text, const RectF &box, SDL_Color iColor );
  
  inline bool isDead() const { return dead; }
  inline Vector2f getPos() const {  return box.pos;  }
  
  void setPos( const Vector2f &pos ) {  box.pos = pos;  }
  
  // Set position of sprite so it's center is @pos (accounting for current size of sprite) 
  void setCenter( const Vector2f &pos ) {  box.setCenter( pos );  }
  
  void setSize( const Vector2f &size ) {  box.size = size;  }
  
  bool exitedWorldBounds();
  void enforceWorldLimits();
	void move( const Vector2f &displacement );
  
  void turnOffParticleCloudSplash();
  
  // ICollideable interface implementation
  inline RectF getHitBox() const override {  return box.scaledCopy( hitBoxScale );  }
  inline Vector2f getCentroid() const override {  return box.centroid();  }
  inline string getName() const override {  return name.c_str();  }
  
  // Every sprite can override update / draw, to act/appear differently.
	virtual void update( float t );
	virtual void draw() const;
  virtual void die();
 
  virtual int getScore() const; 
  
};
