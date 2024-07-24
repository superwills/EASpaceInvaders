#pragma once

#include "Sprite.h"
#include "BulletType.h"

class Bullet : public Sprite {
  inline static Vector2f DefaultBulletScale = Vector2f( .02, .03 );
  BulletType type = BulletType::PlayerNormal;
  
  inline static const map<BulletType, float> BulletSpeeds = {
    // Player bullets go UP, so they have a negative speed
    { BulletType::PlayerNormal, -300 },
    { BulletType::PlayerSpread, -200 },
    
    // expanding box
    { BulletType::PlayerThickLaser, -20 },
    
    // Invader bullets go down, +speed
    { BulletType::InvaderNormal, +100 },
    { BulletType::InvaderSuper, +400 },
  };
  
  inline static const map<BulletType, AnimationId> BulletAnimations = {
    { BulletType::PlayerNormal, AnimationId::BulletPlayerArrow },
    { BulletType::PlayerSpread, AnimationId::BulletPlayerBall },
    { BulletType::PlayerThickLaser, AnimationId::BulletLaser },
    
    { BulletType::InvaderNormal, AnimationId::BulletInvaderArrow },
    { BulletType::InvaderSuper, AnimationId::BulletInvaderLightning },
  };
  
public:
  static bool IsBulletTypeFromInvader( BulletType bulletType );
  
  Bullet( const Vector2f &shootCenter, BulletType bulletType );
  void update( float t ) override;
  
  void updateAnimationType();
  float getBulletSpeed() const;
  bool isFromInvader() const;
  
};
