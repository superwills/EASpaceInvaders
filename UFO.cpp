#include "UFO.h"

#include "Assets.h"

UFO::UFO( const RectF &rectangle ) :
    Sprite( rectangle, AnimationId::UFO ) {
  name = "UFO/" + name;
  collisionType = ICollideableType::UFO;
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
