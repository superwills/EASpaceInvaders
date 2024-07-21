#pragma once

// Unique identifiers for each sound effect. Not using strings to avoid typos.
enum class SFXId {
  Ping0,
  Ping1,
  Ping2,
  Ping3,
  Win,
};

// Gives SFX between low -> high (inclusive)
SFXId randSound( SFXId low, SFXId high );

// Ids for music files.
enum class MusicId {
  Background0,
  Background1,
};

enum class AnimationId {
  E,
  A
};
