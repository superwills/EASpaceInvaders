#include "Sprite.h"
#include "Game.h"
#include "Colors.h"

Sprite::Sprite() {
  name = makeString( "Sprite %d", spriteId );
  
  // Here i'm initializing the size to a non-zero values, so there aren't surprises :)
  box.w = box.h = 64;
}

Sprite::Sprite( const RectF& rectangle ) : box( rectangle ) {
  name = makeString( "Sprite %d", spriteId );
}

Sprite::Sprite( SDL_Texture *iTex ) {
	name = makeString( "Sprite %d from texture", spriteId );
	addAnimationFrame( iTex, White, 1 );
	retrieveTexSize();
}

void Sprite::addAnimationFrame( SDL_Texture *tex, SDL_Color color, float duration ) {
  animation.addFrame( Animation::Frame( tex, color, duration ) );
}

Sprite* Sprite::Text( const string &text, SDL_Color iColor ) {
  SDL_Texture* tex = sdl->makeTextTexture( text, iColor );
  if( !tex ) {
    error( "Sprite::Text couldn't make text texture for `%s`", text.c_str() );
    return 0;
  }
  
	Sprite *sprite = new Sprite( tex );
  return sprite;
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

void Sprite::setCenter( Vector2f pos ) {
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
	hidden = false;
}

void Sprite::hide() {
	hidden = true;
}

void Sprite::update() {
  animation.update( 1./60 );
  enforceWorldLimits();
}

void Sprite::draw() {
  if( hidden ) {
    return; // just don't draw
  }
  
  Animation::Frame &af = animation.getCurrentFrame();
  sdl->setColor( af.color );
	if( !af.tex ) {
		// no texture, so draw a solid rect
		sdl->fillRect( box, af.color );
	}
	else {
		sdl->drawTexture( box, af.tex );
	}
}

void Sprite::retrieveTexSize() {
  Animation::Frame &af = animation.getCurrentFrame();
  
	int w, h;
	SDL_QueryTexture( af.tex, 0, 0, &w, &h );
  box.w=w, box.h=h;
}
