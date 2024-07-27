#include "Invader.h"

#include "Bullet.h"
#include "Game.h"
#include "Log.h"

Invader::Invader( const RectF& rectangle, AnimationId animationId ) :
    Sprite( rectangle, animationId ) {
  name = "Invader/" + name; // prepend Invader/
  collisionType = ICollideableType::Invader;
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

void Invader::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet: {
      // hit by bullet damage or die
      Bullet *bullet = (Bullet*)o;
      // Don't get hit by friendlies
      if( !bullet->isFromInvader() ) {
        die();
      }
    }
    break;
  case ICollideableType::Bunker:
    // do nothing
    break;
  case ICollideableType::BunkerPiece:
    break;
  case ICollideableType::Invader:
    break;
  case ICollideableType::Item:
    break;
  case ICollideableType::Player:
    // nothing happens to me
    break;
  case ICollideableType::UFO:
    break;
    
  case ICollideableType::NotCollideable:
  default:
    error( "Colliding with non-collideable" );
    break;
  }
}


void Invader::tryShoot() const {  
  BulletType bulletType = BulletType::InvaderNormal;
  if( withChance( ChanceForSuperBullet ) ) {
    bulletType = BulletType::InvaderSuper;
  }
  
  game->tryShootBullet( bulletType, box.midBottom() );
}
