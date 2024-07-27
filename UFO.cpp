#include "UFO.h"

#include "Assets.h"
#include "SDLWindow.h"

UFO::UFO() {
  name = "UFO/" + name;
  collisionType = ICollideableType::UFO;
  setAnimation( AnimationId::UFO );
  Vector2f windowSize = sdl->getWindowSize();
  
  box.size = windowSize * DefaultSizePercent;
  box.pos.x = windowSize.x; // start offscreen right
  box.pos.x--; // 1 pixel in to be sure it doesn't immediately die for being offscreen
  box.pos.y = box.size.y / 2;  
  velocity.x = -DefaultUFOSpeed;
  deathSound = SFXId::Explode2;
  
  particleCloudProperties.setNumParticles( 10, 20 );
  particleCloudProperties.setSizes( 14, 25 );
  
  sfxChannel = sdl->loopSound( SFXId::UFO );
  
  if( withChance( .75 ) ) {
    itemDrop = AnimationId::ItemThickLaser;
  }
}

UFO::~UFO() {
  if( sfxChannel != -1 ) {
    Mix_HaltChannel( sfxChannel );
  }
}

void UFO::update( float t ) {
  Sprite::update( t ) ;
  
  if( exitedWorldBounds() ) {
    // Here we shortcut all the usual in the object dying and just remove it
    dead = 1;
  }
}

void UFO::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet:
    die();
    break;
  case ICollideableType::Bunker:
    break;
  case ICollideableType::BunkerPiece:
    break;
  case ICollideableType::Invader:
    break;
  case ICollideableType::Item:
    break;
  case ICollideableType::Player:
    break;
  case ICollideableType::UFO:
    break;
    
  case ICollideableType::NotCollideable:
  default:
    error( "Colliding with non-collideable" );
    break;
  }
}


int UFO::getScore() const {
  int baseScore = Sprite::getScore();
  
  return baseScore * randInt( 1, 5 );
}
