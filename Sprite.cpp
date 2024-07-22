#include "Sprite.h"

#include "Colors.h"
#include "Game.h"
#include "Texture.h"

Sprite::Sprite() {
  name = makeString( "Sprite %d", spriteId );
  box.size = 64;  // init nonzero, so there aren't surprises
}

Sprite::Sprite( const RectF& rectangle ) :
    box( rectangle ) {
  name = makeString( "Sprite %d", spriteId );
}

Sprite::Sprite( const RectF& rectangle, SDL_Color color ) :
    box( rectangle ) {
  name = makeString( "Sprite %d", spriteId );
  
  addSolidColorAnimationFrame( color );
}

Sprite::Sprite( const RectF& rectangle, shared_ptr<Texture> iTex, SDL_Color color ) :
    box( rectangle ) {
	name = makeString( "Sprite %d", spriteId );
 
  // We need at least 1 animation frame to represent the static tex. 
	addAnimationFrame( iTex, color );
}

Sprite::Sprite( const RectF& rectangle, AnimationId animationId ) :
    box( rectangle ), character( animationId ) {
  
  name = makeString( "Sprite %d animation %d", spriteId, animationId );
  animation = sdl->getAnimation( animationId );
}

void Sprite::addSolidColorAnimationFrame( SDL_Color color ) {
  animation.addFrame( Animation::Frame( 0, color ) );
}

void Sprite::addAnimationFrame( shared_ptr<Texture> tex, SDL_Color color ) {
  animation.addFrame( Animation::Frame( tex, color ) );
}

void Sprite::addAnimationFrame( shared_ptr<Texture> tex, SDL_Color color, float duration ) {
  animation.addFrame( Animation::Frame( tex, color, duration ) );
}

void Sprite::addAnimationFrame( shared_ptr<Texture> tex, const RectF &subRect, SDL_Color color, float duration ) {
  animation.addFrame( Animation::Frame( tex, subRect, color, duration ) );
}

shared_ptr<Sprite> Sprite::Text( const string &text, const RectF &box, SDL_Color iColor ) {
  shared_ptr<Texture> tex = sdl->makeTextTexture( text );
  shared_ptr<Sprite> textSprite = std::make_shared<Sprite>( box, tex, iColor );
  textSprite->minParticles = textSprite->maxParticles = 0;
  return textSprite;
}

Vector2f Sprite::getPos() {
	return box.pos;
}

void Sprite::setPos( const Vector2f &pos ) {
  box.pos = pos;
}

void Sprite::setCenter( const Vector2f &pos ) {
  box.setCenter( pos );
}

Vector2f Sprite::getCenter() {
	return box.centroid();
}

void Sprite::setSize( const Vector2f &size ) {
  box.size = size;
}

void Sprite::scale( float s ) {
  box.size *= s;
}

void Sprite::enforceWorldLimits() {
	if( box.top() < 0 ) {
		float overshot = 0 - box.top();
    box.pos.y += overshot;
	}
  
  Vector2f windowSize = sdl->getWindowSize();
	if( box.bottom() > windowSize.y ) {
		float overshot = windowSize.y - box.bottom();
    box.pos.y += overshot;
	}
  
	// ensure stays within bounds of world
	// two of these can happen simultaneously, which is why no else is used
	if( box.left() < 0 ) {
		float overshot = - box.left();
    box.pos.x += overshot;
	}
  
	if( box.right() > windowSize.x ) {
		float overshot = windowSize.x - box.right();
    box.pos.x += overshot;
	}
}

void Sprite::show() {
	hidden = 0;
}

void Sprite::hide() {
	hidden = 1;
}

void Sprite::move( float x, float y ) {
  box.pos.x += x;
  box.pos.y += y;
}

bool Sprite::hit( const Vector2f &p ) {
  return box.hit( p );
}

bool Sprite::hit( shared_ptr<Sprite> other ) {
  return box.hit( other->box );
}

RectF Sprite::getDrawBox() const {
  return box;
}

void Sprite::update( float t ) {
  animation.update( t );
  
  box.pos += velocity*t; // Multiply by t allows timescale
}

void Sprite::draw() const {
  if( hidden ) {
    return; // just don't draw
  }
  
  const Animation::Frame &animFrame = animation.getCurrentFrame();
  if( !animFrame.tex ) {
		// no texture, so draw a solid rect
		sdl->fillRect( getDrawBox(), animFrame.color );
	}
	else {
		//sdl->drawTexture( box, animFrame.tex );
    sdl->draw( getDrawBox(), animFrame );  
	}
}

void Sprite::die() {
  if( dead ) {
    error( "%s double die call", name.c_str() );
  }
  
  // mark for removal at end of frame.
  // The reason for doing this is it is awkward to remove from collections while iterating
  // (which is often when we discover the object died)
  dead = 1;
  
  // ask for a particle splash from the Game. 
  // No particles if vars set to 0.
  if( minParticles && maxParticles ) {
    game->particleSplash( box.centroid(), randInt( minParticles, maxParticles ) );
  }
}

int Sprite::getScore() const {
  auto it = ScoreForCharacter.find( character );
  
  if( it == ScoreForCharacter.end() ) {
    error( "Character %d has no score assigned to it", character );
    return 0;
  }
  
  return it->second;
}
