#pragma once

#include "Sprite.h"

class BunkerPiece : public Sprite {
public:
  
  BunkerPiece( const RectF &rectangle, SDL_Color color );
  
  void onHit( shared_ptr<ICollideable> o ) override;
  
};
