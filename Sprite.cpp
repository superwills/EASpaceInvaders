#include "Sprite.h"

#include "Colors.h"
#include "Game.h"
#include "Texture.h"

Sprite::Sprite() {
  name = makeString( "Sprite %d", spriteId );
  
  // Here i'm initializing the size to a non-zero values, so there aren't surprises :)
  box.w = box.h = 64;
}

Sprite::Sprite( const RectF& rectangle ) : box( rectangle ) {
  name = makeString( "Sprite %d", spriteId );
}

Sprite::Sprite( shared_ptr<Texture> iTex ) {
	name = makeString( "Sprite %d from texture", spriteId );
	addAnimationFrame( iTex, White, 1 );
  
	box.w = box.h = 64;
}

Sprite::Sprite( const RectF& rectangle, AnimationId animationId ) :
    box( rectangle ) {
  
  name = makeString( "Sprite %d animation %d", spriteId, animationId );
  animation = sdl->getAnimation( animationId );
}

void Sprite::addBlankAnimationFrame() {
  animation.addFrame( Animation::Frame( 0, White ) );
}

void Sprite::addAnimationFrame( shared_ptr<Texture> tex, SDL_Color color, float duration ) {
  animation.addFrame( Animation::Frame( tex, color, duration ) );
}

void Sprite::addAnimationFrame( shared_ptr<Texture> tex, const RectF &subRect, SDL_Color color, float duration ) {
  animation.addFrame( Animation::Frame( tex, subRect, color, duration ) );
}

shared_ptr<Sprite> Sprite::Text( const string &text, SDL_Color iColor ) {
  shared_ptr<Texture> tex = sdl->makeTextTexture( text, iColor );
  return std::make_shared<Sprite>( tex );
}

Vector2f Sprite::getPos() {
	return box.xy();
}

void Sprite::setPos( const Vector2f &pos ) {
  box.xy() = pos;
}

Vector2f Sprite::getCenter() {
	return box.centroid();
}

void Sprite::setCenter( const Vector2f &pos ) {
  box.setCenter( pos );
}

void Sprite::setSize( const Vector2f &size ) {
  box.size() = size;
}

void Sprite::scale( float s ) {
  box.size() *= s;
}

void Sprite::enforceWorldLimits() {
	if( box.top() < 0 ) {
		float overshot = 0 - box.top();
    box.y += overshot;
	}
  
  Vector2f windowSize = sdl->getWindowSize();
	if( box.bottom() > windowSize.y ) {
		float overshot = windowSize.y - box.bottom();
    box.y += overshot;
	}
  
	// ensure stays within bounds of world
	// two of these can happen simultaneously, which is why no else is used
	if( box.left() < 0 ) {
		float overshot = - box.left();
    box.x += overshot;
	}
  
	if( box.right() > windowSize.x ) {
		float overshot = windowSize.x - box.right();
    box.x += overshot;
	}
}

void Sprite::show() {
	hidden = 0;
}

void Sprite::hide() {
	hidden = 1;
}

void Sprite::move( float x, float y ) {
  box.x += x, box.y += y;
}

RectF Sprite::getDrawBox() const {
  return box;
}

void Sprite::update( float t ) {
  animation.update( t );
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
  // mark for removal at end of frame.
  // The reason for doing this is it is awkward to remove from collections while iterating
  // (which is often when we discover the object died)
  dead = 1;
}
