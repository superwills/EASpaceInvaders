#include "Assets.h"

#include "randomUtil.h"


SFXId randSound( SFXId low, SFXId high ) {
  int sfxId = randIntIn( (int)low, (int)high );
  return (SFXId)sfxId;
}
