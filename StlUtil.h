#pragma once

#include <vector>
using namespace std;

#include "Log.h"

// A helper template to avoid repeating this code formula for each collection
// Works on Sprite class derivatives (must have T.dead member)
template <typename T> void clearDeadOnes( vector<T> &v ) {
  v.erase( std::remove_if( v.begin(), v.end(), []( T o ) {
    return o->dead;
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

string makeString( const char *fmt, va_list args ) ;

string makeTime( double s ) ;


