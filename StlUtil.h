#pragma once

#include <vector>
#include <string>
#include <memory>
using std::string, std::vector;

// A helper template to avoid repeating this code formula for each collection
// Works on Sprite class derivatives (must have T.isDead() member function)
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

template <typename T>
void append( vector<T> &dst, const vector<T> &src ) {
  dst.insert( dst.end(), src.begin(), src.end() );
}

template <typename T>
bool isAnyOf( const T &value, const vector<T> &anyOfThese ) {
  return std::find( anyOfThese.begin(), anyOfThese.end(), value ) != anyOfThese.end();
}

#define DECLARE_SHARED_PTR(TYPE) typedef std::shared_ptr<TYPE> SP_##TYPE

// shared_from_this() for derived classes: defines a fn shared_CLASS, returning shared_ptr<CLASS>
// A base class of CLASS must derived from enable_shared_from_this 
#define DECLARE_SHARED_DERIVED( CLASS ) shared_ptr<CLASS> shared_##CLASS() { return static_pointer_cast<CLASS>( shared_from_this() ); }

// eg DECLARE_SHARED_DERIVED( Item ) declares:
//shared_ptr<Item> shared_Item() { return dynamic_pointer_cast<Item>( shared_from_this() ); }
