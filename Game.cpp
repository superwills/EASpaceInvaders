#include "Game.h"
#include "Colors.h"

#include "Invader.h"
#include "Player.h"

Game* game = 0;

Game::Game() {
	gameState = GameState::Title;
  
  title = new TitleScreen( "space invaders!" );
  pausedText = Sprite::Text( "pause", SDL_ColorMake( 200, 200, 0, 200 ) );
  pausedText->setCenter( sdl->getSize()/2 );

	bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
  
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
  
	player = new Player( Vector2f( 25, 25 ), 10 );
	
	// Set initial positions for player paddles
	float centerH = sdl->getSize().y/2.f - player->box.h/2;
	player->setPos( Vector2f( sdl->getSize().x - player->box.w, centerH ) );
	
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
	
	setState( GameState::Title );
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
  case GameState::Title:
		// start the title music
		sdl->playMusic( Music::Background0 );
	 	break;
	
  case GameState::Running:
  case GameState::Won:
		// game song
		// if prevstate was Running, don't restart the music
		if( prevState == GameState::Paused ) {
			Mix_ResumeMusic(); // unpause the music
		}
		break;
	
  case GameState::Paused:
		Mix_PauseMusic();
		break;
	
  case GameState::Exiting:
		break;
	}

	gameState = newState;
}

void Game::togglePause() {
	if( gameState == GameState::Paused ) {
		setState( prevState ); //go back to the previous state
		pausedText->hide();
	}
	else {
		setState( GameState::Paused );
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
	
  if( ball->box.hit( player->box ) ) {
    sdl->playSound( randSound( SFX::Ping0, SFX::Ping3 ) );
		float overlap = player->box.left() - ball->box.right();
		ball->box.x += overlap;
		
		float y = player->box.centroid().y - ball->box.centroid().y;
		float a = M_PI + M_PI/2.f * y/player->box.h;
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
	player->move( controller.mouseX );
	//|| controller.keystate[SDL_SCANCODE_LEFT/RIGHT]

	// let the game objects update themselves
	player->update();
	ball->update();
  
  for( Invader *invader : invaders ) {
    
  }
	
	// Check for collisions after each object moves
	checkForCollisions();
}

void Game::update() {
	// Get controller inputs first:
	controller.update();

	if( gameState == GameState::Title )	{
		title->update();
	}
	else if( gameState == GameState::Won ) {
		flashesRem--;
		// change the color only every few frames
    if( every( flashesRem, 3 ) ) {
      bkgColor = SDL_RandomSolidColor();
		}
		if( !flashesRem ) {
			bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
			setState( GameState::Running );
		}
	}
	else if( gameState == GameState::Running ) {
		runGame();
	}
}

void Game::draw() {
	sdl->setColor( bkgColor );

	SDL_RenderClear( sdl->renderer );
	
	if( gameState == GameState::Title ) {
		title->draw();
	}
	else {
		player->draw();
		ball->draw();
		leftScoreSprite->draw();
		rightScoreSprite->draw();
	}
	
	if( gameState == GameState::Paused ) {
		pausedText->draw();
	}
  
  for( Invader* invader : invaders ) {
    invader->draw();
  }
	
	SDL_RenderPresent( sdl->renderer );
}

