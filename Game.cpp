#include "Game.h"

#include "Bullet.h"
#include "Colors.h"
#include "Invader.h"
#include "Player.h"

Game *game = 0;

void Game::init() {

  bkgColor = Indigo;
  
  title = std::make_shared<TitleScreen>( "space invaders!" );
  
  pausedText = Sprite::Text( "pause", SDL_ColorMake( 200, 200, 0, 200 ) );
  pausedText->setCenter( sdl->getWindowSize()/2 );
  
  // Load sprite animations
  sdl->loadSpritesheetAnimation( AnimationId::A, "assets/ims/A.png", 4, Vector2f( 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::E, "assets/ims/E.png", 4, Vector2f( 16 ) );
  
	// load the sfx. These sfx were created with SFXR
	// http://www.drpetter.se/project_sfxr.html
	sdl->loadWavSound( SFXId::Ping0, "assets/sounds/ping0.wav" );
	sdl->loadWavSound( SFXId::Ping1, "assets/sounds/ping1.wav" );
	sdl->loadWavSound( SFXId::Ping2, "assets/sounds/ping2.wav" );
	sdl->loadWavSound( SFXId::Ping3, "assets/sounds/ping3.wav" );
	sdl->loadWavSound( SFXId::Win, "assets/sounds/win.wav" );
  
  // https://www.stef.be/bassoontracker
  sdl->loadMusic( MusicId::Background0, "assets/sounds/1721341344111_2337.mod.mp3" );
	
	setState( GameState::Title );
  changeScore( 0 ); // Create the scoresprite for the 1st time
}

bool Game::isState( GameState state ) {
  return gameState == state;
}

Game::GameState Game::getState() {
	return gameState;
}

void Game::setState( GameState newState ) {
	prevState = gameState;
	
	switch( newState ) {
  case GameState::Title:
		sdl->playMusic( MusicId::Background0 );
	 	break;
	
  case GameState::Running: {
      if( prevState == GameState::Title ) {
        initGameBoard();
      }
    }
    break;
      
  case GameState::Won:
  case GameState::Lost:
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

void Game::initGameBoard() {
  player = std::make_shared<Player>( sdl->getWindowSize() );
  
  // The board is 80% of the window size.
  RectF gameBoard( Vector2f( 0 ), sdl->getWindowSize() );
  gameBoard.y += gameBoard.h/5; // Move down 1/5 screen
  gameBoard.size() *= .8; // 80% of screen
  
  invaderGroup.populate( gameBoard );
}

void Game::changeScore( int byScoreValue ) {
  score += byScoreValue;
  
  // kill the old score
  if( scoreSprite ) {
    scoreSprite->die();
  }
  
  // Unfortunately not efficient due to not using tex atlases..
  scoreSprite = Sprite::Text( makeString( "%d", score ), White );
  scoreSprite->scale( 0.48f );
  scoreSprite->setCenter( sdl->getWindowSize().x/2, scoreSprite->box.h/2 );
}

void Game::checkForCollisions() {
	// check bullets against invaders
  for( shared_ptr<Bullet> bullet : allBullets ) {
    for( shared_ptr<Invader> invader : invaderGroup.invaders ) {
      if( bullet->box.hit( invader->box ) ) {
        bullet->die();
        sdl->playSound( rand<SFXId>( SFXId::Ping0, SFXId::Ping3 ) );
        
        changeScore( invader->scoreValue );
        invader->die();
      }
    }
  }
}

void Game::clearDead() {
  
  allBullets.erase( std::remove_if( allBullets.begin(), allBullets.end(), []( shared_ptr<Bullet> bullet ) {
    return bullet->dead;
  } ), allBullets.end() );
  
  invaderGroup.clearDead();
}

void Game::runGame() {
  invaderGroup.update( dt );
  player->update( dt );
  scoreSprite->update( dt );

  for( shared_ptr<Bullet> bullet : allBullets ) {
    bullet->update( dt );
  }
	
	// Check for collisions after each object moves
	checkForCollisions();
 
  // See if the invaders won by reaching the bottom
  if( invaderGroup.didInvadersWin() ) {
    info( "Invaders won by reaching the bottom" );
    gameState = GameState::Lost;
  } 
 
  clearDead(); 
}

void Game::controllerUpdate() {
  // All states do these:
	controller.update();
   
  // p during play pauses the game.
  if( controller.justPressed( SDL_SCANCODE_P ) ) {
    game->togglePause();
  }
  
  if( gameState == GameState::Running ) {
    // Use the controller state to change gamestate
    player->move( controller.mouseX, 0 );
    
    if( controller.isPressed( SDL_SCANCODE_LEFT ) ) {
      player->move( -1, 0 );
    }
    if( controller.isPressed( SDL_SCANCODE_RIGHT ) ) {
      player->move( 1, 0 );
    }
    player->enforceWorldLimits();
    
    if( controller.justPressed( SDL_SCANCODE_SPACE ) ) {
      shared_ptr<Bullet> bullet = player->shoot();
      allBullets.push_back( bullet );
    }
  }
}

void Game::update() {
  // FrameTime is the difference between current clock time and 
  dt = clock.sec() - clockThisFrame;
  clockThisFrame = clock.sec();

  controllerUpdate();
  
  // State-specific update
  switch( gameState ) {
  case GameState::Title:
  case GameState::Won:
  case GameState::Lost:
  case GameState::Exiting:
		title->update( dt );
	  break;
    
	case GameState::Running:
    runGame();
	  break;
	
  case GameState::Paused: 
		pausedText->update( dt );
	  break;
  }
  
  controller.clearEventKeys();
}

void Game::draw() {
	sdl->clear( bkgColor );

	switch( gameState ) {
  case GameState::Title:
  case GameState::Won:
  case GameState::Lost:
  case GameState::Exiting:
		title->draw();
	  break;
    
	case GameState::Running:
    invaderGroup.draw();
		player->draw();
		scoreSprite->draw();
  
    for( shared_ptr<Bullet> bullet : allBullets ) {
      bullet->draw();
    }
	  break;
	
  case GameState::Paused: 
		pausedText->draw();
	  break;
  }
	
	sdl->present();
}

