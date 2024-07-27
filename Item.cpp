#include "Item.h"

#include "Bullet.h"

Item::Item( const Vector2f &center, AnimationId animationId ) {
  box.size = sdl->getWindowSize() * DefaultSizePercent;
  box.setCenter( center );
  
  velocity = Vector2f( 0, 50 );
  
  setAnimation( animationId );
}

void Item::onHit( shared_ptr<ICollideable> o ) {
  switch( o->getType() ) {
  case ICollideableType::Bullet: {
      // laser obliterates items
      shared_ptr<Bullet> bullet = dynamic_pointer_cast<Bullet>( o );
      if( bullet->type == BulletType::PlayerThickLaser ) {
        die();
      }
    }
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
    die();
    break;
  case ICollideableType::UFO:
    break;
    
  default:
    break;
  }
}
