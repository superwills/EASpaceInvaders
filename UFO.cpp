#include "UFO.h"

#include "Assets.h"

UFO::UFO( const RectF &rectangle ) : Sprite( rectangle, AnimationId::UFO ) {
  name = "UFO/" + name;
  velocity.x = -500;
  deathSound = SFXId::Explode2;
}
