#include "Test.h"

#include "randomUtil.h"
#include "Sprite.h"

Test::Test() {
  RectF windowRectangle = sdl->getWindowRectangle();
  
  RectF largeSpriteRectangle;
  largeSpriteRectangle.size = windowRectangle.size * .33;
  largeSpriteRectangle.setCenter( windowRectangle.size / 2 );
  largeSprite = std::make_shared<Sprite>( largeSpriteRectangle, Yellow );
  sprites.push_back( largeSprite );
  
  for( int i = 0; i < 500; i++ ) {
    RectF r( windowRectangle.randomPoint(), Vector2f( 16 ) );
    
    auto smallSprite = std::make_shared<Sprite>( r, White );
    if( largeSprite->hit( smallSprite ) ) {
      smallSprite->animation.frames.front().color = Blue;
    }
  
    sprites.push_back( smallSprite );
  }
}

void Test::testMouseHit( const Vector2f &p ) {
  if( largeSprite->hit( p ) ) {
    largeSprite->animation.frames.front().color = Red;
  }
  else {
    largeSprite->animation.frames.front().color = Yellow;
  }
}

void Test::update( float t ) {
  for( auto sprite : sprites ) {
    sprite->update( t );
  }
}

void Test::draw() {

  for( auto sprite : sprites ) {
    sprite->draw();
  }

}
