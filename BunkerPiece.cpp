#include "BunkerPiece.h"

BunkerPiece::BunkerPiece( const RectF &rectangle, SDL_Color color ) :
    Sprite( rectangle, color ) {
  name = "BunkerPiece/" + name;
  collisionType = ICollideableType::BunkerPiece;
  deathSound = SFXId::ExplodeBunker;
  
  if( withChance( .1 ) ) {
    itemDrop = AnimationId::ItemPlus1;
  }
  else if( withChance( .05 ) ) {
    itemDrop = AnimationId::ItemShield;
  }
}

void BunkerPiece::onHit( ICollideable *o ) {
  switch( o->collisionType ) {
  case ICollideableType::Bullet:
    die();
    break;
  case ICollideableType::Bunker:
    break;
  case ICollideableType::BunkerPiece:
    break;
  case ICollideableType::Invader:
    die();
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
