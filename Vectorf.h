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

	inline float max() const { return std::max( x,y ) ; }
	inline float min() const { return std::min( x,y ) ; }
	
  inline Vector2f operator+( const Vector2f& o ) const {
		return Vector2f(x+o.x,y+o.y);
	}
	inline Vector2f operator-() const {
		return Vector2f(-x,-y);
	}
	inline Vector2f operator-( const Vector2f& o ) const {
		return Vector2f(x-o.x,y-o.y);
	}

	inline Vector2f operator*( const Vector2f& o ) const {
		return Vector2f(x*o.x,y*o.y);
	}
	inline Vector2f operator*( float s ) const {
		return Vector2f(x*s,y*s);
	}

	inline Vector2f operator/( const Vector2f& o ) const {
		return Vector2f(x/o.x,y/o.y).checkNaN() ;

	}
	inline Vector2f operator/( float s ) const {
		return Vector2f(x/s,y/s).checkNaN() ;
	}
	inline float cross( const Vector2f& o ) const {
		return x*o.y-y*o.x ;
	}
	inline float dot( const Vector2f& o ) const {
		return x*o.x+y*o.y ;
	}
	inline float len() const {
		return sqrtf( x*x+y*y ) ;
	}
	inline float len2() const {
		return x*x+y*y ;
	}
	inline Vector2f& setLen( float iLen ){
		if( float length = len() ) {
			*this *= iLen / length;
		}
		return *this;
	}
	inline Vector2f& setAngle( float a ){
		// put on x-axis
		x = len();
		
		// now rotate
		float newX = x * cosf( a );
		float newY = x * sinf( a );
		x=newX, y=newY;
		return *this;
	}
	inline Vector2f normalizedCopy() const {
		return Vector2f( *this ).normalize() ;
	}
	inline float angleWith( const Vector2f& o ) const {
		return acosf( this->normalizedCopy().dot(o.normalizedCopy()) ) ;
	}
	// Returns + if this leads o.
	// more expensive than unsigned angle,
	// (2x atan2f)
	inline float signedAngleWith( const Vector2f& o ) const {
		float aThis = atan2f( y, x );
		float aO = atan2f( o.y, o.x ) ;

		//info( "lead=%.2f lag=%.2f, ", aThis, aO ) ;
		return aThis - aO ;
		// When 
	}
	// proximity
	inline bool isNear( const Vector2f& o ) const{
		return fabsf(x-o.x)<EPS_MIN && fabsf(y-o.y)<EPS_MIN ;
	}
	inline bool isNear( const Vector2f& o, const Vector2f& maxDiff ) const{
		return fabsf(x-o.x)<maxDiff.x && fabsf(y-o.y)<maxDiff.y ;
	}
	inline bool isCanonical() const {
		return -1.f <= x && x <= 1.f && 
			-1.f <= y && y <= 1.f ;
	}
  #ifdef __APPLE__
  #define _isnan isnan
  #endif
	inline bool isNaN() const {
		return _isnan(x) || _isnan(y) ;
	}
	
  inline Vector2f& checkNaN() {
		// nan is toxic, avoid program corruption by overwriting NaN with HUGE
		if( _isnan( x ) )
			x = HUGE ;
		if( _isnan( y ) )
			y = HUGE ;
		return *this ;
	}
  
	inline bool operator==( const Vector2f& o ) const {
		return x==o.x && y==o.y ;
	}
	inline bool operator!=( const Vector2f& o ) const {
		return x!=o.x || y!=o.y ;
	}
  
	inline Vector2f& normalize() {
		float length = len() ;

		// I added this debug check man, never take it out.
		if( !length ) {
			error( "Vector2f::normalize() attempt to divide by 0" ) ;
			return *this ;
		}

		return (*this)/=length ;
	}
	inline float safeNormalize() {
		float length = len() ;
		if( length )  (*this)/=length ;
		return length ;
	}
	inline Vector2f& operator+=( const Vector2f& o ) {
		x+=o.x,y+=o.y;
		return *this ;
	}
	inline Vector2f& operator-=( const Vector2f& o ) {
		x-=o.x,y-=o.y;
		return *this ;
	}
	inline Vector2f& operator*=( const Vector2f& o ) {
		x*=o.x,y*=o.y;
		return *this ;
	}
	inline Vector2f& operator*=( float s ) {
		x*=s,y*=s;
		return *this ;
	}
	inline Vector2f& operator/=( const Vector2f& o ) {
		x/=o.x,y/=o.y;
		return checkNaN() ;
	}
	inline Vector2f& operator/=( float s ) {
		x/=s,y/=s;
		return checkNaN() ;
	}

	inline Vector2f& operator%=( float s ) {
		if( !s ) {
			error( "mod by 0" ) ;
			return *this ;
		}
		x=fmodf( x,s );  y=fmodf( y,s );
		return *this ;
	}
	inline Vector2f& operator%=( const Vector2f &o ) {
		if( !o.x )  error( "mod by 0 (x)" ) ;
		else x=fmodf( x,o.x );
		if( !o.y )  error( "mod by 0 (y)" ) ;  
		else y=fmodf( y,o.y );
		return *this ;
	}

	inline Vector2f& clampLen( float maxLen ) {
		float length = len() ;
		if( length > maxLen ) // also means length > 0, hopefully
			return normalize()*=maxLen ;

		return *this ;
	}
	inline Vector2f& clampComponent( float minVal, float maxVal ) {
		::clamp( x,minVal,maxVal ) ;
		::clamp( y,minVal,maxVal ) ;
		return *this ;
	}
	inline Vector2f& fabs() {
		x=fabsf(x) ;  y=fabsf(y);
		return *this ;
	}
 
  string toString() const {
    return makeString( "%.2f, %.2f", x, y );
  } 
} ;

inline Vector2f operator-( const Vector2f& v, float s ) {
	return Vector2f(v.x-s,v.y-s);
}

inline Vector2f operator-( float s, const Vector2f& v ) {
	return Vector2f(s-v.x,s-v.y);
}

