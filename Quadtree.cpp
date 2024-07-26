#include "Quadtree.h"

#include "Game.h"

void QuadtreeNode::constructChildren() {
  if( hasChildren() ) {
    error( "Children were already constructed" );
    return;
  }

  Vector2f size = bounds.size/2;
  children.resize( N );
  
  for( int i = 0; i < children.size(); i++ ) {
    children[ i ] = std::make_shared<QuadtreeNode>();
    children[ i ]->bounds.size = size;   // All same size.
    
    children[ i ]->color = color * 2;
  }
  
  // +-----+
  // |TL|TR|
  // +--+--+
  // |BL|BR|
  // +-----+
  children[ TopLeft ]->bounds.pos = bounds.pos;
  children[ TopRight ]->bounds.pos = bounds.midTop();
  children[ BottomLeft ]->bounds.pos = bounds.midLeft();
  children[ BottomRight ]->bounds.pos = bounds.mid();
}

void QuadtreeNode::pushObjectsDown() {
  for( auto obj : objects ) {
    // Push the object 
    for( int i = 0; i < children.size(); i++ ) {
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
  
  // Find which child it belongs in
  if( hasChildren() ) {
    // push responsibility off to children.
    for( int i = 0; i < children.size(); i++ ) {
      if( children[ i ]->contains( obj ) ) {
        children[ i ]->add( obj ); // may go in more than 1 for straddling objects
      }
    }
  }
  else {
    objects.push_back( obj );
    
    // once this node gets too "full", you can subdivide it
    if( objects.size() >= MaxObjects ) {
      constructChildren();
      pushObjectsDown();
    }
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
    for( int i = 0; i < children.size(); i++ ) {
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
    for( int i = 0; i < children.size(); i++ ) {
      children[ i ]->query( box, results );
    }
  }
}

void QuadtreeNode::draw( int depth ) const {
  sdl->rectOutline( bounds.copy().pad( -2*depth ), color );
  
  for( int i = 0; i < children.size(); i++ ) {
    children[ i ]->draw( depth + 1 );
  }
}

void QuadtreeNode::each( const std::function<void( QuadtreeNode* )> &fn ) {
  fn( this );
  
  for( int i = 0; i < children.size(); i++ ) {
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
  root.draw( 0 );
}
