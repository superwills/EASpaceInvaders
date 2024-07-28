#include "Item.h"

#include "Bullet.h"

Item::Item( const Vector2f &center, AnimationId animationId ) {
  name = "Item/" + name;
  collisionType = ICollideableType::Item;
  box.size = sdl->getWindowSize() * DefaultSizePercent;
  box.setCenter( center );
  
  velocity = Vector2f( 0, 50 );
  
  setAnimation( animationId );
}

void Item::update( float t ) {
  Sprite::update( t );
  
  // If the item leaves the bounds, then it dies.
  if( !sdl->getWindowRectangle().hit( box ) ) {
    die();
  }
  
}

void Item::die() {
  Sprite::die();
}

void Item::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet: {
      // laser obliterates items
      Bullet *bullet = (Bullet*)o;
      if( bullet->isBulletType( BulletType::PlayerThickLaser ) ) {
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
  
  case ICollideableType::NotCollideable:
  default:
    error( "Colliding with non-collideable" );
    break;
  }
}
