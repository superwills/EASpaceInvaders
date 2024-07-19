#include "Game.h"
#include "Colors.h"

#include "Invader.h"

Game* game = 0;

Game::Game() {
	gameState = Title;

	bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
	flashesRem = 0;
  
  // Lay the invaders out
  const int rowsOfInvaders = 5;
  const int invadersPerRow = 11; // 11 across in the original game.
  float boardSize = sdl->winWidth * .8;
  float invaderSize = boardSize / invadersPerRow;
  
  for( int row = 0; row < rowsOfInvaders; row++ ) {
    for( int i = 0; i < invadersPerRow; i++ ) {
      RectF box;
      box.y = (rowsOfInvaders - row) * ( invaderSize + 5 );
      box.x = i*( invaderSize + 5 );
      
      box.w = box.h = invaderSize;
      invaders.push_back( new Invader( box ) );
    }
  }
	leftPaddle = new Paddle( Vector2f(25,100), 10 );
	rightPaddle = new Paddle( Vector2f(25,100), 10 );
	title = new TitleScreen( "space invaders!" );
	pausedText = Sprite::Text( "pause", SDL_ColorMake( 200, 200, 0, 200 ) );
	pausedText->setCenter( sdl->getSize()/2 );

	// Set initial positions for player paddles
	float centerH = sdl->getSize().y/2.f - leftPaddle->box.h/2;
	leftPaddle->setPos( Vector2f( leftPaddle->getPos().x, centerH ) );
	rightPaddle->setPos( Vector2f( sdl->getSize().x - rightPaddle->box.w, centerH ) );
	
	// Create the ball
	ball = new Ball( 32 );
	resetBall();
	ball->saveLastStartSpeed();
	
	// load the sfx. These sfx were created with SFXR
	// http://www.drpetter.se/project_sfxr.html
	sdl->loadWavSound( SFX::Ping0, "assets/sounds/ping0.wav" );
	sdl->loadWavSound( SFX::Ping1, "assets/sounds/ping1.wav" );
	sdl->loadWavSound( SFX::Ping2, "assets/sounds/ping2.wav" );
	sdl->loadWavSound( SFX::Ping3, "assets/sounds/ping3.wav" );
	sdl->loadWavSound( SFX::Win, "assets/sounds/win.wav" );
	
	setState( Title );
}

void Game::leftPlayerScored() {
	leftScoreValue++;
	resetBall();
	gameState = JustScored;
	sdl->playSound( SFX::Win );
	flashesRem = 60;
}

void Game::rightPlayerScored() {
	rightScoreValue++;
	resetBall();
	gameState = JustScored;
	sdl->playSound( SFX::Win );
	flashesRem = 60;
}

bool Game::isState( GameState state ) {
  return gameState == state;
}

Game::GameState Game::getState() {
	return gameState;
}

void Game::setState( GameState newState ) {
	prevState = gameState;
	
	switch( newState )
	{
	case Title:
		// start the title music
		sdl->playMusic( Music::Background0 );
	 	break;
	
	case Running:
	case JustScored:
		// game song
		// if prevstate was Running, don't restart the music
		if( prevState == Paused ) {
			Mix_ResumeMusic(); // unpause the music
		}
		break;
	
	case Paused:
		Mix_PauseMusic();
		break;
	
	case Exiting:
		break;
	}

	gameState = newState;
}

void Game::togglePause() {
	if( gameState == Paused ) {
		setState( prevState ); //go back to the previous state
		pausedText->hide();
	}
	else {
		setState( Paused );
		pausedText->show();
	}
}

void Game::resetBall() {
	ball->setCenter( sdl->getSize()/2 );
	ball->vel = Vector2f::random(-1, 1);
	ball->vel.setLen( ball->lastStartSpeed * 2.f );
	drawScores();
}

void Game::drawScores() {
	if( leftScoreSprite ) {
		delete leftScoreSprite;
	}
	leftScoreSprite = Sprite::Text( makeString( "%d", leftScoreValue ), White );
	leftScoreSprite->scale( 0.48f );
	leftScoreSprite->setCenter( sdl->getSize().x/2 - leftScoreSprite->box.w,
		leftScoreSprite->box.h/2 );

	if( rightScoreSprite ) {
		delete rightScoreSprite;
	}
	rightScoreSprite = Sprite::Text( makeString( "%d", rightScoreValue ), White );
	rightScoreSprite->scale( 0.48f );
	rightScoreSprite->setCenter( sdl->getSize().x/2 + rightScoreSprite->box.w,
		rightScoreSprite->box.h/2 );
}

void Game::checkForCollisions() {
	bool ballHit = false;
	// check the ball's rect against the paddle's rects
	if( ball->box.hit( leftPaddle->box ) ) {
    int sfxId = (int)SFX::Ping0 + randInt( 0, 2 );
		sdl->playSound( (SFX)sfxId );
		
		// Push the ball off the paddle, so they don't interpenetrate
		float overlap = leftPaddle->box.right() - ball->box.left();
		ball->box.x += overlap;

		// let the bounce angle be proportional to distance from center paddle
		float y = ball->box.centroid().y - leftPaddle->box.centroid().y;
		float a = M_PI/2.f * y/leftPaddle->box.h;
		ball->vel.setAngle( a );
		
		ballHit = true;
	}

  if( ball->box.hit( rightPaddle->box ) ) {
    sdl->playSound( randSound( SFX::Ping0, SFX::Ping3 ) );
		float overlap = rightPaddle->box.left() - ball->box.right();
		ball->box.x += overlap;
		
		float y = rightPaddle->box.centroid().y - ball->box.centroid().y;
		float a = M_PI + M_PI/2.f * y/rightPaddle->box.h;
		ball->vel.setAngle( a );
		
		ballHit = true;
	}

	// when the ball is hit the ball bounces and speeds up a bit
	if( ballHit )	{
		ball->vel *= 1.2f;
	}
}

void Game::runGame() {
	// Use the controller state to change gamestate
	if( controller.mouseY < 0 || controller.keystate[SDL_SCANCODE_UP] )
		rightPaddle->moveUp();
	else if( controller.mouseY > 0 || controller.keystate[SDL_SCANCODE_DOWN] )
		rightPaddle->moveDown();

	if( controller.keystate[SDL_SCANCODE_W] )
		leftPaddle->moveUp();
	if( controller.keystate[SDL_SCANCODE_S] )
		leftPaddle->moveDown();
	
	// let the game objects update themselves
	leftPaddle->update();
	rightPaddle->update();
	ball->update();
  
  for( Invader *invader : invaders ) {
    
  }
	
	// Check for collisions after each object moves
	checkForCollisions();
}

void Game::update() {
	// Get controller inputs first:
	controller.update();

	if( gameState == Title )	{
		title->update();
	}
	else if( gameState == JustScored ) {
		// Now if we're in the "JustScored" state, then
		// the action pauses for a bit while the screen flashes
		flashesRem--;
		// change the color only every few frames
    if( every(flashesRem,3) ) {
			bkgColor = SDL_ColorMake( randInt(0,255), randInt(0,255), randInt(0,255), randInt(0,255) );
		}
		if( !flashesRem ) {
			bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
			setState( Running );
		}
	}
	else if( gameState == Running ) {
		runGame();
	}
}

void Game::draw() {
	// Set the background's color,
	sdl->setColor( bkgColor );

	// clears the bkg to bkgColor
	SDL_RenderClear( sdl->renderer );
	
	if( gameState == Title ) {
		title->draw();
	}
	else {
		leftPaddle->draw();
		rightPaddle->draw();
		ball->draw();
		leftScoreSprite->draw();
		rightScoreSprite->draw();
	}
	
	if( gameState == Paused ) {
		pausedText->draw();
	}
  
  for( Invader* invader : invaders ) {
    invader->draw();
  }
	
	SDL_RenderPresent( sdl->renderer );
}

