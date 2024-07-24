#include "Invader.h"

#include "Game.h"
#include "Log.h"

Invader::Invader( const RectF& rectangle, AnimationId animationId ) : Sprite( rectangle, animationId ) {
  name = "Invader/" + name; // prepend Invader/
  animation = sdl->getAnimation( animationId );
  deathSound = SFXId::ExplodeEnemy;
  deathAnimation = AnimationId::Explode;
  
  // 20% chance to have a drop
  if( withChance( ChanceToDropItem ) ) {
    itemDrop = rand<AnimationId>( AnimationId::ItemPlus1, AnimationId::ItemShield );
  }
}

void Invader::update( float t ) {
  Sprite::update( t );
  enforceWorldLimits();
  
  // Don't shoot when at the title
  if( game->isState( GameState::Running ) ) {
    // shoot with chance. what limits shots is the Game checking how many bullets there are out
    if( withChance( ChanceToShoot ) ) {
      tryShoot();
    }
  }
}

void Invader::tryShoot() const {  
  BulletType bulletType = BulletType::InvaderNormal;
  if( withChance( ChanceForSuperBullet ) ) {
    bulletType = BulletType::InvaderSuper;
  }
  
  game->tryShootBullet( bulletType, box.midBottom() );
}
