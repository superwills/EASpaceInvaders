#pragma once

// This file defines unique ID's for each loaded asset. This makes it easy to avoid
// double-loading assets. Ids instead of strings to avoid issues from typos

// This lightweight header can be included anywhere you need to refer to assets by id.

enum class SFXId {
  Ping0,
  Ping1,
  Ping2,
  Ping3,
  Win,
};

enum class MusicId {
  Background0,
  Background1,
};

enum class AnimationId {
  Invader1,
  Invader2,
  A,
  E,
  Explode,
  UFO,
};

