#include "Game.h"

#include "Bullet.h"
#include "Bunker.h"
#include "Colors.h"
#include "GameOverScreen.h"
#include "Invader.h"
#include "Particle.h"
#include "Player.h"
#include "TitleScreen.h"
#include "UFO.h"

Game *game = 0;

void Game::init() {

  bkgColor = Indigo;
  
  // Load sprite animations
  sdl->loadSpritesheetAnimation( AnimationId::Invader1, "assets/ims/invader-1.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Invader2, "assets/ims/invader-2.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::InvaderBullet1, "assets/ims/invaderBullet1.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::InvaderBullet2, "assets/ims/invaderBullet2.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::A, "assets/ims/A.png", 4, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::E, "assets/ims/E.png", 4, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::UFO, "assets/ims/ufo.png", 2, Vector2f( 32, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::MenuPointer, "assets/ims/menu-pointer.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Explode, "assets/ims/explode.png", 5, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Player, "assets/ims/player.png", 1, Vector2f( 16, 16 ) );
  
  title = std::make_shared<TitleScreen>( "space invaders!" );
  gameOverScreen = std::make_shared<GameOverScreen>();
  
  RectF pausedTextBox = sdl->getWindowRectangle();
  pausedTextBox.size /= 3;
  pausedTextBox.setCenter( sdl->getWindowSize() / 2 );
  
  pausedText = Sprite::Text( "pause", pausedTextBox, SDL_ColorMake( 200, 200, 0, 200 ) );
  
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
  Vector2f windowSize = sdl->getWindowSize();
  
  RectF playerBox;
  playerBox.size = windowSize.x * .08;  // % of world size.
  playerBox.pos.x = windowSize.x/2 - playerBox.size.x/2;
  playerBox.pos.y = windowSize.y - playerBox.size.y;
  
  player = std::make_shared<Player>( playerBox );
  
  // The board is 80% of the window size.
  RectF gameBoard = sdl->getWindowRectangle();
  gameBoard.pos.y += gameBoard.size.y/5; // Move down 1/5 screen
  gameBoard.size *= .5;
  
  invaderGroup.populate( gameBoard );
  
  // Bunker size is so 9 can fit across, but only every other one is filled in with a bunker
  Vector2f bunkerSize;
  bunkerSize.x = windowSize.x / 9;
  bunkerSize.y = bunkerSize.x * .6;
  
  for( int i = 0; i < 9; i++ ) {
    if( i % 2 == 0 ) {
      continue; // skip every other one.
    }
    Vector2f bunkerPos;
    bunkerPos.x = bunkerSize.x * i;
    bunkerPos.y = windowSize.y - 2*bunkerSize.y;
    RectF bunkerRectangle( bunkerPos, bunkerSize );
    
    shared_ptr<Bunker> bunker = std::make_shared<Bunker>( bunkerRectangle );
    allBunkers.push_back( bunker );
  }
}

void Game::genUFO() {
  nextUFO -= dt;
  if( nextUFO < 0 ) {
    nextUFO = UFOInterval;
    
    RectF ufoBox;
    ufoBox.size = Vector2f( 64, 32 );
    ufoBox.pos.x = sdl->getWindowSize().x; // start offscreen right.
    ufoBox.pos.y = ufoBox.size.y / 2;
    shared_ptr<UFO> ufo = std::make_shared<UFO>( ufoBox );
    allUFOs.push_back( ufo );
  }
}

void Game::shootBullet( const RectF &source, bool fromInvader, const Vector2f &vel ) {
  shared_ptr<Bullet> bullet = std::make_shared<Bullet>( source, fromInvader );
  bullet->vel = vel;
  allBullets.push_back( bullet );
}

void Game::particleSplash( const Vector2f &pos, int numParticles ) {
  for( int i = 0; i < numParticles; i++ ) {
    Vector2f size = Vector2f::random( 4, 12 );
    shared_ptr<Particle> p = std::make_shared<Particle>( RectF( pos, Vector2f( size ) ) );
    allParticles.push_back( p );
  }
}

void Game::changeScore( int byScoreValue ) {
  score += byScoreValue;
  
  // kill the old score
  if( scoreSprite ) {
    scoreSprite->die();
  }
  
  // Top center.
  Vector2f windowSize = sdl->getWindowSize();
  RectF scoreSpriteBox;
  scoreSpriteBox.size = windowSize / 10;
  scoreSpriteBox.pos.x = windowSize.x/2 - scoreSpriteBox.size.x/2;
  scoreSpriteBox.pos.y = 0; // top.
  
  scoreSprite = Sprite::Text( makeString( "%d", score ), scoreSpriteBox, White );
}

void Game::checkForCollisions() {
	// check bullets against invaders, ufo's, bunkers.
  for( auto bullet : allBullets ) {
  
    // First check against bunkers.
    for( auto bunker : allBunkers ) {
      if( bunker->hit( bullet ) ) {
        bullet->die();
        break;
      }
    }
    
    if( bullet->dead ) {
      continue;
    }
  
    if( bullet->fromInvader ) {
      // bullets from invaders can't hit other invaders or the ufos.
      // check against the player and that's all it does
      if( bullet->box.hit( player->box ) ) {
        player->die();
      } 
      continue;
    }
    
    for( auto invader : invaderGroup.invaders ) {
      if( bullet->box.hit( invader->box ) ) {
        bullet->die();
        sdl->playSound( rand<SFXId>( SFXId::Ping0, SFXId::Ping3 ) );
        
        changeScore( invader->getScore() );
        invader->die();
        
        particleSplash( invader->box.centroid(), randInt( 5, 8 ) );
        break;
      }
    }
    
    if( bullet->dead ) {
      // If the bullet was consumed, skip the ufo check
      continue;
    }
    
    for( auto ufo : allUFOs ) {
      if( bullet->box.hit( ufo->box ) ) {
        bullet->die();
        sdl->playSound( rand<SFXId>( SFXId::Ping0, SFXId::Ping3 ) );
        
        changeScore( ufo->getScore() );
        ufo->die();
        
        particleSplash( ufo->box.centroid(), randInt( 8, 12 ) );
      }
    }
  }
}

void Game::clearDead() {
  clearDeadOnes( allBullets );
  clearDeadOnes( allParticles );
  clearDeadOnes( allUFOs );
  for( auto bunker : allBunkers ) {
    bunker->clearDead();
  }
  
  invaderGroup.clearDead();
}

void Game::runGame() {
  
  invaderGroup.update( dt );
  player->update( dt );
  scoreSprite->update( dt );
  genUFO();

  for( auto bullet : allBullets ) {
    bullet->update( dt );
  }
	
  for( auto particle : allParticles ) {
    particle->update( dt );
  }
  
  for( auto ufo : allUFOs ) {
    ufo->update( dt );
  }
  
  for( auto bunker : allBunkers ) {
    bunker->update( dt );
  }
	
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
    
    float mX = 2;
    if( controller.isPressed( SDL_SCANCODE_LEFT ) ) {
      player->move( -mX, 0 );
    }
    if( controller.isPressed( SDL_SCANCODE_RIGHT ) ) {
      player->move( mX, 0 );
    }
    player->enforceWorldLimits();
    
    if( controller.justPressed( SDL_SCANCODE_SPACE ) ) {
      shootBullet( player->box, 0, Vector2f( 0, -100 ) );
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
    title->update( dt );
    break;
  case GameState::Won:
  case GameState::Lost:
    gameOverScreen->update( dt );
    break;
  case GameState::Exiting:
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
    title->draw();
    break;
  case GameState::Won:
  case GameState::Lost:
    gameOverScreen->draw();
    break;
  case GameState::Exiting:
	  break;
    
	case GameState::Running:
    invaderGroup.draw();
		player->draw();
		scoreSprite->draw();
  
    for( auto bullet : allBullets ) {
      bullet->draw();
    }
    for( auto particle : allParticles ) {
      particle->draw();
    }
    
    for( auto ufo : allUFOs ) {
      ufo->draw();
    }
    
    for( auto bunker : allBunkers ) {
      bunker->draw();
    }
	  break;
	
  case GameState::Paused: 
		pausedText->draw();
	  break;
  }
	
	sdl->present();
}

