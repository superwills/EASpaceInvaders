#pragma once

#include "Sprite.h"

#include "BulletType.h"
#include "StlUtil.h"

class Bullet : public Sprite {
  inline static Vector2f DefaultBulletScale = Vector2f( .02, .03 );
  
  inline static const map<BulletType, float> BulletSpeeds = {
    // Player bullets go UP, so they have a negative speed
    { BulletType::PlayerNormal, -300 },
    { BulletType::PlayerSpread, -200 },
    
    // the laser doesn't actually move, it has an expanding box
    { BulletType::PlayerThickLaser, 0 },
    
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
  BulletType type = BulletType::PlayerNormal;
  
public:
  
  static bool IsBulletTypeFromInvader( BulletType bulletType );
  
  Bullet( const Vector2f &shootCenter, BulletType bulletType );
  DECLARE_SHARED_DERIVED( Bullet )
  
  inline bool isBulletType( BulletType bulletType ) const { return type == bulletType; }
  void update( float t ) override;
  
  void onHit( ICollideable *o ) override;
  
  bool isFromInvader() const;
  void updateAnimationType();
  float getBulletSpeed() const;
  
};

DECLARE_SHARED_PTR( Bullet );
