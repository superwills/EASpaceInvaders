#pragma once

#include "ICollideable.h"
#include "Log.h"
#include "SDLWindow.h"

#include <functional>

struct QuadtreeNode {
  RectF bounds;
  
  enum ChildNode { TopRight, TopLeft, BottomLeft, BottomRight };
  inline static const int N = 4;
  vector< shared_ptr<QuadtreeNode> > children;
  int MaxObjects = 25;
  SDL_Color color = Red;
  
  vector< shared_ptr<ICollideable> > objects;
  
  QuadtreeNode() { }
  QuadtreeNode( const RectF &rect ) : bounds( rect ) { }
  
  inline bool hasChildren() const {  return !children.empty();  }
  inline bool contains( shared_ptr<ICollideable> obj ) const {
    return bounds.hit( obj->getHitBox() );
  }
  
  void constructChildren();
  
  void pushObjectsDown();
  
  bool add( shared_ptr<ICollideable> obj );
  
  void remove( shared_ptr<ICollideable> obj );
  
  // Pass the `box` rather than regenerating it multiple times
  void query( const RectF &box, vector< shared_ptr<ICollideable> > &results );
  
  void draw() const;
  
  void each( const std::function<void( QuadtreeNode* )> &fn );
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
  Quadtree( const RectF &bounds );
  
  void each( const std::function<void( QuadtreeNode* )> &fn );
  
  void add( shared_ptr<ICollideable> obj );
  
  void remove( shared_ptr<ICollideable> obj );
  
  // Gives you list of all objects that possibly intersect `box`
  vector< shared_ptr<ICollideable> > query( shared_ptr<ICollideable> obj );
  
  //debug
  void draw();
};
