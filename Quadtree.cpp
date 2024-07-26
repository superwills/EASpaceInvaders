#include "Quadtree.h"

void QuadtreeNode::constructChildren() {
  Vector2f mid = bounds.mid();
  
  for( int i = 0; i < N; i++ ) {
    children[ i ] = std::make_shared<QuadtreeNode>();
    children[ i ]->bounds.size = mid;   // All same size.
    
    children[ i ]->color = color * 2;
  }
  
  children[ TopRight ]->bounds.pos.x = mid.x;
  children[ BottomLeft ]->bounds.pos.y = mid.y;
  children[ BottomRight ]->bounds.pos = mid;
}

void QuadtreeNode::pushObjectsDown() {
  for( auto obj : objects ) {
    if( !contains( obj ) )  continue;
    
    // Push each child down a level.
    for( int i = 0; i < N   &&   children[ i ]; i++ ) {
      if( children[ i ]->contains( obj ) ) {
        children[ i ]->add( obj );
      }
    }
  }
  
  objects.clear();
}

bool QuadtreeNode::add( shared_ptr<ICollideable> obj ) {
  // It didn't fit in the children, so either it misses, or I don't have children yet.
  if( !contains( obj ) ) {
    error( "%s does not intersect quadtree bounds", obj->getName().c_str() );
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

void QuadtreeNode::remove( shared_ptr<ICollideable> obj ) {
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

// Pass the `box` rather than regenerating it multiple times
void QuadtreeNode::query( const RectF &box, vector< shared_ptr<ICollideable> > &results ) {
  if( box.hit( bounds ) ) {
    // You need all the objects in this box
    append( results, objects );
    
    // also check my children
    for( int i = 0; i < N   &&   children[ i ]; i++ ) {
      children[ i ]->query( box, results );
    }
  }
}

void QuadtreeNode::draw() const {
  sdl->rectOutline( bounds.copy().pad( -1 ), color );
  
  for( int i = 0; i < N   &&   children[ i ]; i++ ) {
    children[ i ]->draw();
  }
}

void QuadtreeNode::each( const std::function<void( QuadtreeNode* )> &fn ) {
  fn( this );
  for( int i = 0; i < N   &&   children[ i ]; i++ ) {
    children[ i ]->each( fn ); //recurse
  }
}




Quadtree::Quadtree( const RectF &bounds ) :
    root( bounds ) { }

void Quadtree::each( const std::function<void( QuadtreeNode* )> &fn ) {
  root.each( fn );
}

void Quadtree::add( shared_ptr<ICollideable> obj ) {
  root.add( obj );
  Uint8 base = 50;
  root.color = { base, base, base, 255 };
}

void Quadtree::remove( shared_ptr<ICollideable> obj ) {
  root.remove( obj );
}

// Gives you list of all objects that possibly intersect `box`
vector< shared_ptr<ICollideable> > Quadtree::query( shared_ptr<ICollideable> obj ) {
  // get leaf node that completely contains box.
  // if you straddle two boxes, you have to get the things in both.
  vector< shared_ptr<ICollideable> > results;
  root.query( obj->getHitBox(), results );
  return results;
}

void Quadtree::draw() {
  root.draw();
}
