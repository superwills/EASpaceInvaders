#include "UFO.h"

#include "Assets.h"

UFO::UFO( const RectF &rectangle ) : Sprite( rectangle, AnimationId::UFO ) {
  name = "UFO/" + name;
  velocity.x = -500;
  deathSound = SFXId::Explode2;
  
  minParticles = 10, maxParticles = 20;
  particleSizeMin = 14, particleSizeMax = 25;
}
