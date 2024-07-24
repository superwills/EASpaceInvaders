#pragma once

// This file defines unique ID's for each loaded asset. This makes it easy to avoid
// double-loading assets. Ids instead of strings to avoid issues from typos

// This lightweight header can be included anywhere you need to refer to assets by id.

#include <map>
using std::map;

enum class SFXId {
  NoSFX,
  Blip,
  
  Explode2,
  ExplodeEnemy,
  ExplodePlayer,
  ExplodeBunker,
  
  GameStart,
  Select1,
  Shik,
  Shoot1,
  Shoot2,
  Shoot3,
  
  Jump,
  UFO,
};

enum class MusicId {
  NoMusic,
  Background0,
  Background1,
};

// These also uniquely identify object types in the game
enum class AnimationId {
  NoAnimation,
  
  Invader1,
  Invader2,
  InvaderE,
  InvaderA,
  
  BulletInvaderArrow,
  BulletInvaderLightning,
  BulletPlayerArrow,
  BulletPlayerBall,
  BulletLaser,
  
  Explode,
  MenuPointer,
  UFO,
  Player,
  PlayerDie,
  Boss,
  Shield,
  
  ItemPlus1,  //+1 bullet
  ItemSpread, //spreads bullets
  ItemShield,
  ItemThickLaser,
};

// This could have been added to class Sprite, but it seemed to crowd it a little.
extern map<AnimationId, int> ScoreForCharacter;

