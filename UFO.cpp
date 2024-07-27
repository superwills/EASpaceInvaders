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

int UFO::getScore() const {
  int baseScore = Sprite::getScore();
  
  return baseScore * randInt( 1, 5 );
}
