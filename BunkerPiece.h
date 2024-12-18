#pragma once

#include "Sprite.h"

class BunkerPiece : public Sprite {
public:
  
  BunkerPiece( const RectF &rectangle, SDL_Color color );
  
  void onHit( ICollideable *o ) override;
  
};

DECLARE_SHARED_PTR( BunkerPiece );
