#pragma once

#include "randomUtil.h"
#include "Vectorf.h"

#if __APPLE__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

struct RectF {
  // pos is TOP LEFT of rectangle.
  Vector2f pos, size;
  
  inline static int BoxHits = 0;
  
  RectF() { }
  RectF( float x, float y, float w, float h ) :
    pos( x, y ), size( w, h ) { }
    
  RectF( const Vector2f &position, const Vector2f &theSize ):
    pos( position ), size( theSize ) { }
    
  RectF copy() const {
    return RectF( *this );
  }
  
  // The top left corner is the origin.
  inline float top() const { return pos.y; } 
  inline float bottom() const { return pos.y + size.y; }
  
  inline float left() const { return pos.x; }
  inline float right() const { return pos.x + size.x; }
  
  inline float midX() const { return pos.x + size.x/2; }
  inline float midY() const { return pos.y + size.y/2; }
  inline Vector2f mid() const { return Vector2f( midX(), midY() ); }
  
  inline Vector2f bottomLeft() const {
    return Vector2f( left(), bottom() ) ;
  }
  inline Vector2f bottomRight() const {
    return Vector2f( right(), bottom() ) ;
  }
  inline Vector2f topLeft() const {
    return Vector2f( left(), top() ) ;
  }
  inline Vector2f topRight() const {
    return Vector2f( right(), top() ) ;
  }
  inline Vector2f centroid() const {
    return Vector2f( midX(), midY() ) ;
  }
  inline Vector2f midTop() const {
    return Vector2f( midX(), top() );
  }
  inline Vector2f midBottom() const {
    return Vector2f( midX(), bottom() );
  }
  inline Vector2f midLeft() const {
    return Vector2f( left(), midY() );
  }
  inline Vector2f midRight() const {
    return Vector2f( right(), midY() );
  }
  
  inline RectF& setCenter( const Vector2f &newCenter ) {
    // position is going to be offset by half the width,
    // +---+
    // | * |
    // +---+
    pos = newCenter - size / 2;
    return *this;
  }
  
  // +pad grows, -pad shrinks
  inline RectF& pad( const Vector2f &amount ) {
    pos -= amount / 2;  // pull pos back by half the pad amount
    size += amount;     // grow the rectangle by the growth amount
    return *this ;
  }
  
  // Scales it from the center.
  inline RectF scaledCopy( const Vector2f &scale ) const {
    RectF copy( *this );
    
    Vector2f newSize = size*scale;
    
    // pad by the difference. If newSize is smaller, box shrinks
    return copy.pad( newSize - size );
  }
  
  // Random point within the rectangle.
  Vector2f randomPoint() const {
    return Vector2f( randFloat( pos.x, pos.x + size.x ), randFloat( pos.y, pos.y + size.y ) ) ;
  }
  
  inline bool hit( const Vector2f& v ) const {
    ++BoxHits;
    // +-------------->
    // | +---+
    // | |*  | 
    // | +---+ 
    // v
    // A little bit tricky since we have +y going down:
    //   v.y <= bottom() since the bottom side has a bigger value than the top.
    return ( v.x >= left() && v.x <= right() && v.y >= top() && v.y <= bottom() );
  }
  
  inline bool hit( const RectF& o ) const {
    ++BoxHits;
    // Easy to check the miss situations
    // +-------------->
    // | +---+
    // | |   | 
    // | +---+ 
    // |       +---+      
    // |       |   |
    // v       +---+
    //
    bool didHit = !( right() < o.left()   ||  // right edge of this box LEFT OF left edge of other,
                                              // so, this box is completely to the right of the other one
      left() > o.right()   ||
      bottom() < o.top()   ||                 // bottom of this box clears top of other 
      top() > o.bottom() );
    
    return didHit;
  }
  
  inline bool contains( const RectF& r ) const {
    ++BoxHits;
    return hit( r.bottomLeft() ) && hit( r.topRight() );
  }
  
  // "Adding" to a rectangle moves it.
  inline RectF operator+( const Vector2f& offset ) {
    return RectF( pos.x + offset.x, pos.y + offset.y, size.x, size.y ) ;
  }
  
  // For offsetting a rectangle by a vector.
  inline RectF& operator+=( const Vector2f& v ) {
    pos.x += v.x;
    pos.y += v.y;
    return *this ;
  }
  
  inline SDL_Rect toSDLRect() const {
    return { (int)pos.x, (int)pos.y, (int)size.x, (int)size.y };
  }
  
  inline SDL_FRect toSDLFRect() const {
    return { pos.x, pos.y, size.x, size.y };
  }
  
  string toString() const {
    return makeString( "l=%.2f r=%.2f b=%.2f t=%.2f (w=%.2f, h=%.2f)",
      left(), right(), bottom(), top(), size.x, size.y ) ;
  }
};
