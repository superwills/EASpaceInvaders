#include "GameOverScreen.h"

#include "Sprite.h"
#include "Invader.h"

GameOverScreen::GameOverScreen() {
  shared_ptr<Sprite> gameOver = Sprite::Text( "Game over!", RectF( 0, 0, 256, 256 ), Green );
  
  textSprites.push_back( gameOver );
  
  for( int i = 0; i < 5; i++ ) {
    shared_ptr<Invader> invader = std::make_shared<Invader>( RectF(0,0,15,15), AnimationId::Invader1 );
    invaders.push_back( invader );
  }
}

void GameOverScreen::update( float t ) {
  // bounce the invaders
  for( auto sprite : textSprites ) {
    sprite->update( t );
  }
}

void GameOverScreen::draw() {
  for( auto sprite : textSprites ) {
    sprite->draw();
  }
}

void GameOverScreen::win() {
  shared_ptr<Sprite> winSprite = Sprite::Text( "You win!", RectF( 0, 0, 256, 256 ), Green );
  
  textSprites.push_back( winSprite );
  
  
}

void GameOverScreen::lose() {
  shared_ptr<Sprite> loseSprite = Sprite::Text( "You lost!", RectF( 0, 0, 256, 256 ), Green );
  
  textSprites.push_back( loseSprite );
  
  
}
