#include "BunkerPiece.h"

BunkerPiece::BunkerPiece( const RectF &rectangle, SDL_Color color ) : Sprite( rectangle, color ) {
  name = "BunkerPiece/" + name;
  deathSound = SFXId::ExplodeBunker;
  if( withChance( .5 ) ) {
    itemDrop = AnimationId::ItemPlus1;
  }
}
