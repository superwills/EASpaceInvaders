#pragma once

#include <stdio.h>
#include <float.h>
#include <math.h>

#include "Log.h"
#include "randomUtil.h"
#include "StlUtil.h"

struct Vector2f {
  inline static float EPS_MIN = 1e-6f; // distance to be considered ~=
	float x, y;
	
	Vector2f() : x(0.f),y(0.f){}
	Vector2f( float ix, float iy ):x(ix),y(iy){}
	Vector2f( float iv ):x(iv),y(iv){}

	inline Vector2f& operator=( float v ) {
		x=y=v;
		return *this ;
	}

	static inline Vector2f random() { return Vector2f( randFloat(), randFloat() ) ;  }

	static inline Vector2f random( float min, float max ) {
		return Vector2f( randFloat( min, max ), randFloat( min, max ) ) ;
	}
 
  inline bool anyZero() const { return !x || !y; }

	inline float max() const { return std::max( x,y ) ; }
	inline float min() const { return std::min( x,y ) ; }
	
  inline Vector2f operator+( const Vector2f& o ) const {
		return Vector2f( x+o.x, y+o.y );
	}
  inline Vector2f& operator+=( const Vector2f& o ) {
		x+=o.x,y+=o.y;
		return *this ;
	}
	 
	inline Vector2f operator-() const {
		return Vector2f(-x,-y);
	}
	inline Vector2f operator-( const Vector2f& o ) const {
		return Vector2f( x - o.x, y - o.y );
	}
  inline Vector2f& operator-=( const Vector2f& o ) {
		x-=o.x,y-=o.y;
		return *this ;
	}
	
	inline Vector2f operator*( const Vector2f& o ) const {
		return Vector2f( x*o.x, y*o.y );
	}
	inline Vector2f operator*( float s ) const {
		return Vector2f( x*s, y*s );
	}
  inline Vector2f& operator*=( const Vector2f& o ) {
		x*=o.x,y*=o.y;
		return *this ;
	}
	inline Vector2f& operator*=( float s ) {
		x*=s,y*=s;
		return *this ;
	}

	inline Vector2f operator/( const Vector2f& o ) const {
    if( o.anyZero() ) {
      error( "divide by 0" );
      return *this;
    }
    
		return Vector2f( x/o.x, y/o.y );
	}
   
	inline Vector2f operator/( float s ) const {
    if( !s ) {
      error( "divide by 0" );
      return *this;
    } 
    
		return Vector2f( x/s, y/s );
	}
  inline Vector2f& operator/=( const Vector2f& o ) {
    if( o.anyZero() ) {
      error( "divide by 0" );
      return *this;
    }
    
    x /= o.x;
    y /= o.y;
		return *this;
	}
	inline Vector2f& operator/=( float s ) {
    if( !s ) {
      error( "divide by 0" );
      return *this;
    }
    
    x/=s, y/=s;
		return *this;
	} 
  
	inline float len() const {
		return sqrtf( x*x+y*y );
	}
	inline float len2() const {
		return x*x+y*y;
	}
	
  inline Vector2f& normalize() {
		float length = len();

		if( !length ) {
			error( "divide by 0" );
			return *this;
		}

		return (*this) /= length;
	} 
  inline Vector2f normalizedCopy() const {
		return Vector2f( *this ).normalize() ;
	}
	
	inline bool operator==( const Vector2f& o ) const {
		return x==o.x && y==o.y ;
	}
	inline bool operator!=( const Vector2f& o ) const {
		return x!=o.x || y!=o.y ;
	}
	
  inline bool isNear( const Vector2f& o ) const {
		return fabsf( x - o.x ) < EPS_MIN && fabsf( y - o.y ) < EPS_MIN;
	}
  
	string toString() const {
    return makeString( "%.2f, %.2f", x, y );
  } 
};

