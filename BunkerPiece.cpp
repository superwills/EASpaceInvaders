#include "BunkerPiece.h"

BunkerPiece::BunkerPiece( const RectF &rectangle, SDL_Color color ) : Sprite( rectangle, color ) {
  name = "BunkerPiece/" + name;
  deathSound = SFXId::ExplodeBunker;
  
  if( withChance( .1 ) ) {
    itemDrop = AnimationId::ItemPlus1;
  }
  else if( withChance( .05 ) ) {
    itemDrop = AnimationId::ItemShield;
  }
}

void BunkerPiece::onHit( shared_ptr<ICollideable> o ) {
  switch( o->getType() ) {
  case ICollideableType::Bullet:
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
    
  default:
    break;
  }
}
