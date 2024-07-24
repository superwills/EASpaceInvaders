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
