#include "GameOverScreen.h"

#include "Sprite.h"
#include "Invader.h"

GameOverScreen::GameOverScreen() {
  Vector2f windowSize = sdl->getWindowSize();
  
  RectF gameOverBox;
  gameOverBox.size = windowSize;
  gameOverBox.size *= .5;
  gameOverBox.setCenter( windowSize * Vector2f( .5, .25 ) );
  gameOverSprite = Sprite::Text( "Game over!", gameOverBox, Green );
}

void GameOverScreen::addInvaders( AnimationId character1, AnimationId character2 ) {
  Vector2f windowSize = sdl->getWindowSize();
  
  // fit 11 across, but only fill every other one
  //           1
  // 01234567890
  // _I_I_I_I_I_
  Vector2f invaderSize( windowSize.x / 11 );
  
  bool useChr1 = 1;
  for( int i = 1; i <= 9; i+=2 ) {
    Vector2f invaderPos;
    invaderPos.x = i*invaderSize.x;
    invaderPos.y = windowSize.y / 2;
    RectF invaderBox( invaderPos, invaderSize );
    invaderBox.pos.y -= invaderSize.y / 2;
    
    AnimationId chr = useChr1 ? character1 : character2;
    useChr1 = !useChr1;
    
    shared_ptr<Invader> invader = std::make_shared<Invader>( invaderBox, chr );
    invaders.push_back( invader );
  }
}

void GameOverScreen::update( float t ) {
  
  gameOverSprite->update( t );
  message->update( t );
  
  // bounce the invaders?
  for( auto invader : invaders ) {
    invader->update( t );  // they may shoot but that's ok
  }
}

void GameOverScreen::draw() {
  gameOverSprite->draw();
  message->draw();
  for( auto invader : invaders ) {
    invader->draw();
  }
}

void GameOverScreen::win() {
  invaders.clear();
  addInvaders( AnimationId::Invader2, AnimationId::UFO );
  Vector2f windowSize = sdl->getWindowSize();
  
  RectF winBox;
  winBox.size = windowSize;
  winBox.size *= .5;
  winBox.setCenter( windowSize * Vector2f( .5, .75 ) );
  message = Sprite::Text( "You won!", winBox, Blue );
  
  shared_ptr<Texture> tex = message->animation.getCurrentFrame().tex;
  message->addAnimationFrame( tex, SDL_ColorMake( 20, 200, 255, 255 ), .25 );
}

void GameOverScreen::lose() {
  invaders.clear();
  addInvaders( AnimationId::Invader1, AnimationId::Invader1 );
  Vector2f windowSize = sdl->getWindowSize();
  
  RectF loseBox;
  loseBox.size = windowSize;
  loseBox.size *= .5;
  loseBox.setCenter( windowSize * Vector2f( .5, .75 ) );
  message = Sprite::Text( "You lost!", loseBox, Yellow );
  
  shared_ptr<Texture> tex = message->animation.getCurrentFrame().tex;
  message->addAnimationFrame( tex, Red, .25 );
}
