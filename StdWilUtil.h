#pragma once

#include <math.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "Log.h"

#define HUGEF 1e10f

#define every(intVal,cycle) (intVal%cycle==0)

template<typename T>
inline bool contains( const list<T>& container, const T& elt ) {
	return find( container.begin(), container.end(), elt ) != container.end() ;
}

template<typename T>
inline bool contains( const vector<T>& container, const T& elt ) {
	return find( container.begin(), container.end(), elt ) != container.end() ;
}

template<typename T,typename S>
inline bool contains( const map<T,S>& container, const T& elt ) {
	return container.find( elt ) != container.end() ;
}

template <typename T>
inline T& clamp( T& x, T minVal, T maxVal ) {
  if( x < minVal ) {
    x = minVal;
  }
	else if( x > maxVal ) {
    x = maxVal;
  }
	return x;
}

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

// to avoid nans/infs
inline float ratio( float wins, float losses ) {
	if( !wins ) return 0.f ; // avoid nan/infs.
	return wins / (wins+losses);
}

//#define cycleFlag( val, MINVAL, MAXVAL ) ( ++val>(MAXVAL)?val=(MINVAL):val )
inline int cycleFlag( int &val, int minVal, int maxVal ) {
	if( ++val > maxVal )
		val = minVal;

	return val;
}

//#define decycleFlag( val, MINVAL, MAXVAL ) ( --((int)val)<(MINVAL)?val=(MAXVAL):val )
inline int decycleFlag( int &val, int minVal, int maxVal ) {
	if( --val < minVal )
		val = maxVal;

	return val;
}

inline int cycleArrayIndex( int &index, size_t arraySize ) {
  if( ++index >= arraySize ) {
    index = 0;  // reset
  }
  
  return index;
}

inline int decycleArrayIndex( int &index, size_t arraySize ) {
  if( --index < 0 ) {
    index = arraySize - 1;  // reset
  }
  
  return index;
}

string makeString( const char *fmt, ... );

string makeString( const char *fmt, va_list args ) ;

string makeTime( double s ) ;


