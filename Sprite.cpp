#include "Sprite.h"

#include "SDLColors.h"
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
  setAnimation( animationId );
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

void Sprite::setAnimation( AnimationId animationId ) {
  character = animationId;
  animation = sdl->getAnimation( animationId );
}

shared_ptr<Sprite> Sprite::Text( const string &text, const RectF &box, SDL_Color iColor ) {
  shared_ptr<Texture> tex = sdl->makeTextTexture( text );
  shared_ptr<Sprite> textSprite = std::make_shared<Sprite>( box, tex, iColor );
  textSprite->turnOffParticleCloudSplash();
  return textSprite;
}

bool Sprite::exitedWorldBounds() {
  return !sdl->getWindowRectangle().hit( box );
}

void Sprite::enforceWorldLimits() {
	if( box.top() < 0 ) {
		float overshot = 0 - box.top();
    move( Vector2f( 0, overshot ) );
	}
  
  Vector2f windowSize = sdl->getWindowSize();
	if( box.bottom() > windowSize.y ) {
		float overshot = windowSize.y - box.bottom();
    move( Vector2f( 0, overshot ) );
	}
  
	// ensure stays within bounds of world
	// two of these can happen simultaneously, which is why no else is used
	if( box.left() < 0 ) {
		float overshot = - box.left();
    move( Vector2f( overshot, 0 ) );
	}
  
	if( box.right() > windowSize.x ) {
		float overshot = windowSize.x - box.right();
    move( Vector2f( overshot, 0 ) );
	}
}

void Sprite::move( const Vector2f &displacement ) {
  box.pos += displacement;
  
  for( auto child : children ) {
    child->move( displacement );
  }
}

void Sprite::turnOffParticleCloudSplash() {
  particleCloudProperties.setNumParticles( 0, 0 );
}

void Sprite::update( float t ) {
  animation.update( t );
  move( velocity*t );
  
  if( dieOnAnimationEnd && animation.isEnded() ) {
    die();
  }
  
  for( auto child : children ) {
    child->update( t );
  }
}

void Sprite::draw() const {
  const Animation::Frame &animFrame = animation.getCurrentFrame();
  if( !animFrame.tex ) {
		// no texture, so draw a solid rect
		sdl->rectFill( box, animFrame.color );
	}
	else {
		sdl->draw( box, animFrame );  
	}
 
  for( auto child : children ) {
    child->draw();
  } 
}

void Sprite::die() {
  if( dead ) {
    error( "%s double die call", name.c_str() );
  }
  
  // mark for removal at end of frame.
  // The reason for doing this is it is awkward to remove from collections while iterating
  // (which is often when we discover the object died)
  dead = true;
  
  // ask for a particle splash from the Game. 
  // No particles if vars set to 0.
  if( particleCloudProperties.isOn() ) {
    game->particleCloud( box, particleCloudProperties );
  }
  
  if( deathSound != SFXId::NoSFX ) {
    sdl->playSound( deathSound );
  }
  
  int score = getScore();
  if( score > 0 ) {
    game->displayScore( score, getCentroid(), White );
    game->changeScore( score );
  }
  
  if( deathAnimation != AnimationId::NoAnimation ) {
    game->playSpriteAnimation( box, deathAnimation );
  }
  
  if( itemDrop != AnimationId::NoAnimation ) {
    game->createItem( box.centroid(), itemDrop );
  }
  
  for( auto child : children ) {
    child->die();
  }
}

int Sprite::getScore() const {
  auto it = ScoreForCharacter.find( character );
  
  if( it == ScoreForCharacter.end() ) {
    ///error( "Character %d has no score assigned to it", character );
    return 0;
  }
  
  return it->second;
}
