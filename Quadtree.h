#pragma once

#include "ICollideable.h"
#include "Log.h"
#include "SDLWindow.h"

struct QuadtreeNode {
  RectF bounds;
  
  enum ChildNode { TopRight, TopLeft, BottomLeft, BottomRight };
  inline static const int N = 4;
  shared_ptr<QuadtreeNode> children[ N ];
  int MaxObjects = 25;
  
  vector< shared_ptr<ICollideable> > objects;
  
  QuadtreeNode() { }
  QuadtreeNode( const RectF &rect ) : bounds( rect ) { }
  
  void constructChildren() {
    Vector2f mid = bounds.mid();
    
    for( int i = 0; i < N; i++ ) {
      children[ i ] = std::make_shared<QuadtreeNode>();
      children[ i ]->bounds.size = mid;   // All same size.
    }
    
    children[ TopRight ]->bounds.pos.x = mid.x;
    children[ BottomLeft ]->bounds.pos.y = mid.y;
    children[ BottomRight ]->bounds.pos = mid;
  }
  
  inline bool contains( shared_ptr<ICollideable> obj ) const {
    return bounds.hit( obj->getHitBox() );
  }
  
  void pushObjectsDown() {
    for( int o = 0; o < objects.size(); o++ ) {
      shared_ptr<ICollideable> obj = objects[ o ];
      if( !contains( obj ) )  continue;
      
      // Push each child down a level.
      for( int i = 0; i < N   &&   children[ i ]; i++ ) {
        children[ i ]->add( obj );
      }
    }
    
    objects.clear();
  }
  
  bool add( shared_ptr<ICollideable> obj ) {
    // It didn't fit in the children, so either it misses, or I don't have children yet.
    if( !contains( obj ) ) {
      error( "Object does not intersect quadtree bounds" );
      return 0;
    }
    
    // Try to push it into a child node first
    // Find which child it belongs in
    for( int i = 0; i < N   &&   children[ i ]; i++ ) {
      if( children[ i ]->contains( obj ) ) {
        children[ i ]->add( obj );
        return 1;
      }
    }
    
    // once this node gets too "full", you can subdivide it
    objects.push_back( obj );
    
    if( objects.size() >= MaxObjects ) {
      constructChildren();
      pushObjectsDown();
    }
    return 1;
  }
  
  void remove( shared_ptr<ICollideable> obj ) {
    // do I have this object?
    auto it = std::find( objects.begin(), objects.end(), obj );
    if( it != objects.end() ) {
      objects.erase( it );
    }
    else {
      // if not perculate the command to search/remove it from wherever it is
      for( int i = 0; i < N   &&   children[ i ]; i++ ) {
        children[ i ]->remove( obj );
      }
    }
  }
  
  void query( const RectF &box, vector< shared_ptr<ICollideable> > &results ) {
    if( box.hit( bounds ) ) {
      // You need all the objects in this box
      append( results, objects );
      
      // also check my children
      for( int i = 0; i < N   &&   children[ i ]; i++ ) {
        children[ i ]->query( box, results );
      }
    }
  }
  
  void draw( SDL_Color color ) {
    sdl->rectOutline( bounds, color );
    
    for( int i = 0; i < N   &&   children[ i ]; i++ ) {
      children[ i ]->draw( SDL_RandomSolidColor() );
    }
  }
};

class Quadtree {
  // CCW around from TR
  enum Node { TopRight, TopLeft, BottomLeft, BottomRight };
  // +-----+
  // |TL|TR|
  // +--+--+
  // |BL|BR|
  // +-----+
  QuadtreeNode root;

public:
  Quadtree( const RectF &bounds ) : root( bounds ) { }
  
  void add( shared_ptr<ICollideable> obj ) {
    root.add( obj );
  }
  
  void remove( shared_ptr<ICollideable> obj ) {
    root.remove( obj );
  }
  
  // Gives you list of all objects that possibly intersect `box`
  vector< shared_ptr<ICollideable> > query( const RectF &box ) {
    // get leaf node that completely contains box.
    // if you straddle two boxes, you have to get the things in both.
    vector< shared_ptr<ICollideable> > results;
    root.query( box, results );
    return results;
  }
  
  //debug
  void draw() {
    root.draw( SDL_RandomSolidColor() );
  }
};
