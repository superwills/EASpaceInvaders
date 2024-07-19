#pragma once

#include <random>

// bigger range than rand()
extern std::default_random_engine MTRandom32;

// Provide global functions for quick randoms from a default object.
void randSeed( uint32_t seed );
void randSeedTime();
void randSeedMemoryAddress();

// rand integer between 0 and UINT_MAX
inline uint32_t randInt() {
  return MTRandom32();
}

// assumes low=0
inline uint32_t randInt( int high ) {
  if( !high ) {
    return high;
  }
  return MTRandom32() % high;
}

// between low and (high-1)
inline uint32_t randInt( int low, int high ) {
  // modulus 0 is an error
  if( low == high ) {
    return low;
  }
  return low + MTRandom32() % (high-low);
}

// between low and high, inclusive of high end
inline uint32_t randIntIn( int low, int highInc ) {
  // modulus 0 is an error
  if( low == highInc ) {
    return low;
  }
  return low + MTRandom32() % (highInc + 1 - low);
}

inline uint32_t randIntInRange( int low, int range ) {
  if( !range ) {
    return low;
  }
  return low + MTRandom32() % (range + 1);
}

// random on [0, 1]
inline float randFloat() {
  return (float)MTRandom32() / UINT32_MAX;
}

inline float randFloat( float low, float high ) {
  return low + ( high - low )*randFloat();
}

// low of 0
inline float randFloat( float high ) {
  return high*randFloat() ;
}

inline int randSign() {
  return -1 + 2*randInt(0, 2) ; // -1+0 or -1+2=+1
}

