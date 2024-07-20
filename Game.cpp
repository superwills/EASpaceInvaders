#include "Game.h"

#include "Bullet.h"
#include "Colors.h"
#include "Invader.h"
#include "Player.h"

Game *game = 0;

void Game::init() {

  bkgColor = SDL_ColorMake( 0, 0, 40, 255 );
  
  title = std::make_shared<TitleScreen>( "space invaders!" );
  allSharedSprites.push_back( title );
  
  pausedText = Sprite::Text( "pause", SDL_ColorMake( 200, 200, 0, 200 ) );
  allSharedSprites.push_back( pausedText );
  
  Vector2f windowSize = sdl->getWindowSize();
  pausedText->setCenter( windowSize/2 );

	// load the sfx. These sfx were created with SFXR
	// http://www.drpetter.se/project_sfxr.html
	sdl->loadWavSound( SFX::Ping0, "assets/sounds/ping0.wav" );
	sdl->loadWavSound( SFX::Ping1, "assets/sounds/ping1.wav" );
	sdl->loadWavSound( SFX::Ping2, "assets/sounds/ping2.wav" );
	sdl->loadWavSound( SFX::Ping3, "assets/sounds/ping3.wav" );
	sdl->loadWavSound( SFX::Win, "assets/sounds/win.wav" );
  
  // https://www.stef.be/bassoontracker
  sdl->loadMusic( Music::Background0, "assets/sounds/1721341344111_2337.mod.mp3" );
	
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
		sdl->playMusic( Music::Background0 );
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
  
  populateInvaders();
}

void Game::populateInvaders() {
  // Lay the invaders out
  const int rowsOfInvaders = 5;
  const int invadersPerRow = 11; // 11 across in the original game.
  float boardSize = sdl->getWindowSize().x * .8;
  float invaderSize = boardSize / invadersPerRow;
  
  for( int row = 0; row < rowsOfInvaders; row++ ) {
    for( int i = 0; i < invadersPerRow; i++ ) {
      RectF box;
      box.y = (rowsOfInvaders - row) * ( invaderSize + 5 );
      box.x = i*( invaderSize + 5 );
      
      box.w = box.h = invaderSize;
      
      shared_ptr<Invader> invader = std::make_shared<Invader>( box );
      allSharedSprites.push_back( invader );
      allInvaders.push_back( invader );
    }
  }
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
    for( shared_ptr<Invader> invader : allInvaders ) {
      if( bullet->box.hit( invader->box ) ) {
        puts( "Hit invader" );
        //sdl->playSound( randSound( SFX::Ping0, SFX::Ping3 ) );
        invader->die();
      }
    }
  }
}

void Game::clearDead() {
  allSharedSprites.erase(
    std::remove_if( allSharedSprites.begin(), allSharedSprites.end(), []( shared_ptr<Sprite> sprite ) {
      return sprite->dead;
    } ), allSharedSprites.end()
  );
  
  allBullets.erase( std::remove_if( allBullets.begin(), allBullets.end(), []( shared_ptr<Bullet> bullet ) {
    return bullet->dead;
  } ), allBullets.end() );
  
  allInvaders.erase( std::remove_if( allInvaders.begin(), allInvaders.end(), []( shared_ptr<Invader> invader ) {
    return invader->dead;
  } ), allInvaders.end() );
}

void Game::runGame() {
	// Use the controller state to change gamestate
	player->move( controller.mouseX );
  
  if( controller.isPressed( SDL_SCANCODE_LEFT ) ) {
    player->move( -1 );
  }
  if( controller.isPressed( SDL_SCANCODE_RIGHT ) ) {
    player->move( 1 );
  }
  
  if( controller.justPressed( SDL_SCANCODE_SPACE ) ) {
    shared_ptr<Bullet> bullet = player->shoot();
    allBullets.push_back( bullet );
  }
	
	// Check for collisions after each object moves
	checkForCollisions();
 
  clearDead(); 
}

void Game::update() {
	controller.update();
  
  // p during play pauses the game.
  if( controller.justPressed( SDL_SCANCODE_P ) ) {
    game->togglePause();
  }
  
  if( gameState == GameState::Running ) {
		runGame();
	}
 
  for( shared_ptr<Sprite> sprite : allSharedSprites ) {
    sprite->update();
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
    for( shared_ptr<Invader> invader : allInvaders ) {
      invader->draw();
    } 
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

