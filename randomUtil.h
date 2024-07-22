#pragma once

#include <random>

// bigger range than rand()
extern std::default_random_engine MTRandom32;

// between 0 -> (highEx - 1)
inline uint32_t randInt( int highEx ) {
  if( !highEx ) {
    return highEx;
  }
  return MTRandom32() % highEx;
}

// between low and (high-1)
inline uint32_t randInt( int lowIn, int highEx ) {
  // modulus 0 is an error
  if( lowIn == highEx ) {
    return lowIn;
  }
  return lowIn + MTRandom32() % ( highEx - lowIn );
}

// between low and high, inclusive of high end
inline uint32_t randIntIn( int lowIn, int highIn ) {
  // modulus 0 is an error
  if( lowIn == highIn ) {
    return lowIn;
  }
  return lowIn + MTRandom32() % ( highIn + 1 - lowIn );
}

// random on [0, 1]
inline float randFloat() {
  return (float)MTRandom32() / MTRandom32.max();
}

// low of 0
inline float randFloat( float high ) {
  return high*randFloat() ;
}

inline float randFloat( float low, float high ) {
  return low + ( high - low )*randFloat();
}

inline int randSign() {
  return -1 + 2*randInt(0, 2) ; // -1+0 or -1+2=+1
}

// Gives random from an enum between low -> high (inclusive)
template <typename Enum>
Enum rand( Enum low, Enum high ) {
  int randomValue = randIntIn( (int)low, (int)high );
  return (Enum)randomValue;
}
