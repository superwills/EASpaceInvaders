#include "Assets.h"

#include "randomUtil.h"


SFX randSound( SFX low, SFX high ) {
  int sfxId = randIntIn( (int)low, (int)high );
  return (SFX)sfxId;
}
