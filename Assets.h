#pragma once

// Unique identifiers for each sound effect. Not using strings to avoid typos.
enum class SFX {
  Ping0,
  Ping1,
  Ping2,
  Ping3,
  Win,
};

// Gives SFX between low -> high (inclusive)
SFX randSound( SFX low, SFX high );

// Ids for music files.
enum class Music {
  Background0,
  Background1,
};

enum class AnimationId {
  E,
  A
};
