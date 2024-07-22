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
  sdl->loadSpritesheetAnimation( AnimationId::Player, "assets/ims/player.png", 1, Vector2f( 16, 8 ) );
  sdl->loadSpritesheetAnimation( AnimationId::PlayerDie, "assets/ims/player-die.png", 6, Vector2f( 20, 12 ) );
  
  title = std::make_shared<TitleScreen>( "space invaders!" );
  gameOverScreen = std::make_shared<GameOverScreen>();
  
  RectF pausedTextBox = sdl->getWindowRectangle();
  pausedTextBox.size /= 3;
  pausedTextBox.setCenter( sdl->getWindowSize() / 2 );
  
  pausedText = Sprite::Text( "pause", pausedTextBox, SDL_ColorMake( 200, 200, 0, 200 ) );
  
	// created with jsfxr https://sfxr.me/
	sdl->loadWavSound( SFXId::Blip, "assets/sounds/blip-5.wav" );
  sdl->loadWavSound( SFXId::Explode2, "assets/sounds/expl-2.wav" );
  sdl->loadWavSound( SFXId::ExplodeEnemy, "assets/sounds/expl-enemy.wav" );
  sdl->loadWavSound( SFXId::ExplodePlayer, "assets/sounds/expl-player.wav" );
  sdl->loadWavSound( SFXId::ExplodeBunker, "assets/sounds/expl-hit-bunker.wav" );
  sdl->loadWavSound( SFXId::GameStart, "assets/sounds/gameStart.wav" );
  sdl->loadWavSound( SFXId::Select1, "assets/sounds/select-1.wav" );
  sdl->loadWavSound( SFXId::Shik, "assets/sounds/shik.wav" );
  sdl->loadWavSound( SFXId::Shoot1, "assets/sounds/shoot-1.wav" );
  sdl->loadWavSound( SFXId::Shoot2, "assets/sounds/shoot-2.wav" );
  sdl->loadWavSound( SFXId::Shoot3, "assets/sounds/shoot-3.wav" );
  sdl->loadWavSound( SFXId::Jump, "assets/sounds/jump.wav" ); 
  
  // https://www.stef.be/bassoontracker
  sdl->loadMusic( MusicId::Background0, "assets/sounds/1721341344111_2337.mod.mp3" );
  sdl->loadMusic( MusicId::Background1, "assets/sounds/1721341431075_1911.mod.mp3" );
  
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
		sdl->playMusic( rand<MusicId>( MusicId::Background0, MusicId::Background1 ) );
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
  
  // odds, 1,3,5,7
  for( int i = 1; i <= 7; i += 2 ) {
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

void Game::checkWinConditions() {
  // player won if all invaders are gone
  if( invaderGroup.empty() ) {
    info( "Player won" );
    gameState = GameState::Won;
    gameOverScreen->win();
  }
  
  // See if the invaders won by reaching the bottom, or player dead and death animation finished.
  if( invaderGroup.didInvadersWin() || ( player->dead && player->animation.isEnded() ) ) {
    info( "Invaders won by reaching the bottom" );
    gameState = GameState::Lost;
    gameOverScreen->lose();
  } 
}

void Game::checkForCollisions() {
	// check bullets against invaders, ufo's, bunkers.
  for( auto bullet : allBullets ) {
  
    // First check against bunkers.
    for( auto bunker : allBunkers ) {
      if( bunker->hit( bullet ) ) {
        sdl->playSound( SFXId::ExplodeBunker );
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
        sdl->playSound( SFXId::ExplodePlayer );
        player->die();
        
        // put in the player death sprite.
        player->animation = sdl->getAnimation( AnimationId::PlayerDie );
      } 
      continue;
    }
    
    for( auto invader : invaderGroup.invaders ) {
      if( bullet->box.hit( invader->box ) ) {
        bullet->die();
        sdl->playSound( SFXId::ExplodeEnemy );
        
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
        sdl->playSound( SFXId::ExplodeEnemy );
        
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
 
  checkWinConditions();
 
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
    // Player can only move if not dead.
    if( !player->dead ) {
  
      // Use the controller state to change gamestate
      player->move( controller.mouseX, 0 );
      
      float mX = 2;
      if( controller.isPressed( SDL_SCANCODE_LEFT ) ) {
        player->move( -mX, 0 );
      }
      if( controller.isPressed( SDL_SCANCODE_RIGHT ) ) {
        player->move( mX, 0 );
      }
      if( controller.justPressed( SDL_SCANCODE_8 ) ) {
        invaderGroup.killAll();
      }
      player->enforceWorldLimits();
      
      if( controller.justPressed( SDL_SCANCODE_SPACE ) ) {
        shootBullet( player->box, 0, Vector2f( 0, -500 ) );
      }
    
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

