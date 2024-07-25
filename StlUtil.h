#pragma once

#include <vector>
#include <string>
using std::string, std::vector;

// A helper template to avoid repeating this code formula for each collection
// Works on Sprite class derivatives (must have T.dead member)
template <typename T> void clearDeadOnes( vector<T> &v ) {
  v.erase( std::remove_if( v.begin(), v.end(), []( T o ) {
    return o->isDead();
  } ), v.end() );
}

template<typename T>
inline bool isValidIndex( int index, const vector<T>& container ) {
  return index >= 0 && index < container.size();
}

inline int cycleArrayIndex( int &index, size_t arraySize ) {
  if( ++index >= arraySize ) {
    index = 0;  // reset
  }
  
  return index;
}

inline int decycleArrayIndex( int &index, size_t arraySize ) {
  if( --index < 0 ) {
    index = (int)arraySize - 1;  // reset
  }
  
  return index;
}

string makeString( const char *fmt, ... );

string makeTimeString( double s ) ;

// Inclusive
template <typename Enum>
bool isBetweenIn( const Enum &val, const Enum &low, const Enum &high ) {
  int v = (int)val;
  
  // low <= v <= high
  return (int)low <= v   &&   v <= (int)high; 
}
