#include "Game.h"

#include "Bullet.h"
#include "Bunker.h"
#include "SDLColors.h"
#include "GameOverScreen.h"
#include "Invader.h"
#include "Particle.h"
#include "Player.h"
#include "TitleScreen.h"
#include "UFO.h"

shared_ptr<Game> game;

void Game::initGameBoard() {
  Vector2f windowSize = sdl->getWindowSize();
  
  // re/create the player
  RectF playerBox;
  playerBox.size = windowSize * Player::DefaultPlayerSizePercent;
  playerBox.pos.x = windowSize.x/2 - playerBox.size.x/2;
  playerBox.pos.y = windowSize.y - playerBox.size.y;
  player = std::make_shared<Player>( playerBox );
  
  RectF invaderBounds = sdl->getWindowRectangle();
  invaderBounds.pos.y += windowSize.y/10; // Move down some
  invaderBounds.size *= DefaultInvaderBoundsSizePercent;
  
  invaderGroup.populate( invaderBounds );
  
  // Bunker size is so 9 can fit across, but only every other one is filled in with a bunker
  Vector2f bunkerSize;
  bunkerSize.x = windowSize.x / 9;
  bunkerSize.y = bunkerSize.x * .6;
  
  // odds, 1,3,5,7
  for( int i = 1; i <= 7; i += 2 ) {
    Vector2f bunkerPos;
    bunkerPos.x = bunkerSize.x * i;
    bunkerPos.y = windowSize.y - 3*bunkerSize.y;
    RectF bunkerRectangle( bunkerPos, bunkerSize );
    
    shared_ptr<Bunker> bunker = std::make_shared<Bunker>( bunkerRectangle );
    allBunkers.push_back( bunker );
  }
  
  setScore( 0 );
}

void Game::clearGameBoard() {
  invaderGroup.killAll( 0 );
  allBullets.clear();
  allBunkers.clear();
  allParticles.clear();
  allUFOs.clear();
}

void Game::togglePause() {
	if( gameState == GameState::Paused ) {
		setState( prevState );
		Mix_ResumeMusic();  
	}
	else {
		setState( GameState::Paused );
		Mix_PauseMusic();  
	}
}

void Game::init() {

  bkgColor = Indigo;
  
  // Load sprite animations
  sdl->loadSpritesheetAnimation( AnimationId::Invader1, "assets/ims/invader-1.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Invader2, "assets/ims/invader-2.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletInvaderArrow, "assets/ims/bullet-arrow.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletInvaderLightning, "assets/ims/bullet-lightning.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletPlayer, "assets/ims/bullet-player.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::InvaderE, "assets/ims/invader-E.png", 4, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::InvaderA, "assets/ims/invader-A.png", 4, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::UFO, "assets/ims/ufo.png", 2, Vector2f( 32, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::MenuPointer, "assets/ims/menu-pointer.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Explode, "assets/ims/explode.png", 5, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Player, "assets/ims/player.png", 1, Vector2f( 16, 8 ) );
  sdl->loadSpritesheetAnimation( AnimationId::PlayerDie, "assets/ims/player-die.png", 6, Vector2f( 20, 12 ), White, 0 );
  sdl->loadSpritesheetAnimation( AnimationId::Boss, "assets/ims/josh.png", 2, Vector2f( 64, 32 ) );
  
  titleScreen = std::make_shared<TitleScreen>( "space invaders!" );
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
  setScore( 0 );
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
    
  case GameState::Paused:
		break;
      
  case GameState::Won:
  case GameState::Lost:
    clearGameBoard();
    break;
	
  case GameState::Exiting:
		break;
  
  case GameState::Test:
    break;  
	}

	gameState = newState;
}

void Game::setKeyJustPressed( uint16_t key ) {
  controller.setKeyJustPressed( key );
}

void Game::setMouseJustClicked( uint16_t mouseButton ) {
  controller.setMouseJustClicked( mouseButton );
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

int Game::getNumBullets( bool isFromInvader ) {
  int num = 0;
  for( auto bullet : allBullets ) {
    if( isFromInvader == bullet->isFromInvader ) {
      num++;
    }
  }
  return num;
}

void Game::tryShootBullet( const RectF &source, bool isFromInvader, const Vector2f &vel ) {
  int numBullets = getNumBullets( isFromInvader );
  int maxBullets = isFromInvader ? invaderGroup.getMaxNumBullets() : Player::DefaultMaxBullets; 
  if( numBullets >= maxBullets ) {
    return;
  }
  
  shared_ptr<Bullet> bullet = std::make_shared<Bullet>( source, vel, isFromInvader );
  allBullets.push_back( bullet );
}

void Game::particleSplash( const Vector2f &pos, int numParticles, float sizeMin, float sizeMax ) {
  for( int i = 0; i < numParticles; i++ ) {
    Vector2f size = Vector2f::random( sizeMin, sizeMax );
    shared_ptr<Particle> p = std::make_shared<Particle>( RectF( pos, Vector2f( size ) ) );
    allParticles.push_back( p );
  }
}

void Game::setScore( int newScore ) {
  score = newScore;
  
  // Top center.
  Vector2f windowSize = sdl->getWindowSize();
  RectF scoreSpriteBox;
  scoreSpriteBox.size = windowSize / 10;
  scoreSpriteBox.pos.x = windowSize.x/2 - scoreSpriteBox.size.x/2;
  scoreSpriteBox.pos.y = 0; // top.
  
  scoreSprite = Sprite::Text( makeString( "%d", score ), scoreSpriteBox, White );
}

void Game::changeScore( int byScoreValue ) {
  setScore( score + byScoreValue );
}

void Game::checkWinConditions() {
  // player won if all invaders are gone
  if( invaderGroup.empty() ) {
    info( "Player won" );
    setState( GameState::Won );
    gameOverScreen->win();
  }
  
  // See if the invaders won by reaching the bottom, or player dead and death animation finished.
  if( invaderGroup.didInvadersWin() || ( player->dead && player->animation.isEnded() ) ) {
    info( "Invaders won by reaching the bottom" );
    setState( GameState::Lost );
    gameOverScreen->lose();
  } 
}

void Game::checkForCollisions() {
	// check bullets against invaders, ufo's, bunkers.
  for( auto bullet : allBullets ) {
  
    // First check against bunkers.
    for( auto bunker : allBunkers ) {
      if( bunker->killHit( bullet ) ) {
        bullet->die();
        break;
      }
    }
    
    if( bullet->dead ) {
      continue;
    }
  
    if( bullet->isFromInvader ) {
      if( !player->dead && bullet->hit( player ) ) {
        bullet->die();
        player->die();
      }
      
      // If it missed the player, we should skip the checks for other invaders + ufos below
      continue;  // there's nothing else for an invader bullet to do
    }
    
    for( auto invader : invaderGroup.invaders ) {
      if( invader->dead ) {
        continue; // can happen if 2 player bullets try to hit same invader
      }
      
      if( bullet->hit( invader ) ) {
        bullet->die();
        invader->die();
        break;
      }
    }
    
    if( bullet->dead ) {
      // If the bullet was consumed by an invader, skip the ufo check that would happen below
      continue;
    }
    
    for( auto ufo : allUFOs ) {
      if( bullet->hit( ufo ) ) {
        bullet->die();
        ufo->die();
      }
    }
  }
  
  // Check invaders against player itself, bunkers
  for( auto invader : invaderGroup.invaders ) {
    if( !player->dead && invader->hit( player ) ) {
      player->die();
    }
    for( auto bunker : allBunkers ) {
      // kill bunker pieces hit by this invader
      bunker->killHit( invader );
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

void Game::controllerUpdateTitle() {
  // any key down at title starts the game.
  if( controller.justPressedAny( startKeys ) || controller.justPressed( SDL_SCANCODE_SPACE ) ) {
    setState( titleScreen->getLaunchState() );
  }
  
  if( controller.justPressed( SDL_SCANCODE_UP ) ) {
    titleScreen->pointerUp();
  }
  
  if( controller.justPressed( SDL_SCANCODE_DOWN ) ) {
    titleScreen->pointerDown();
  }
}

void Game::controllerUpdateRunning() {
  // Player can only move if not dead.
  if( player->dead ) {
    return;
  }

  player->move( Vector2f( controller.getMouseDX(), 0 ) );
  
  if( controller.isPressed( SDL_SCANCODE_LEFT ) ) {
    player->setMovingLeft();
  }
  if( controller.isPressed( SDL_SCANCODE_RIGHT ) ) {
    player->setMovingRight();
  }
  if( controller.justPressed( SDL_SCANCODE_8 ) ) {
    invaderGroup.killAll( 1 );
  }
  player->enforceWorldLimits();
  
  if( controller.justPressed( SDL_SCANCODE_SPACE ) ) {
    player->tryShoot();
  }
}

void Game::controllerUpdate() {
  
	controller.update();
   
  // p during play pauses the game.
  if( controller.justPressed( SDL_SCANCODE_P ) ) {
    game->togglePause();
  }
  
  if( controller.isPressed( SDL_SCANCODE_K ) ) {
    engineIterationPerFrame = 10;
  }
  else {
    engineIterationPerFrame = 1;
  }
  
  switch( gameState ) {
  case GameState::Title:
    controllerUpdateTitle();
    break;
  
  case GameState::Running:
    controllerUpdateRunning();
    break;
    
  case GameState::Paused:
    break;
  case GameState::Won:
  case GameState::Lost:
    {
      if( controller.justPressedAny( startKeys ) ) {
        setState( GameState::Title );
      }
    } 
    break;
    
  case GameState::Exiting:
    break;
  case GameState::Test: {
      test.testMouseHit( controller.getMousePos() );
      
      if( controller.justPressedAny( startKeys ) ) {
        setState( GameState::Title );
      }
    }
    break;
  }
}

void Game::update() {
  
  // difference in time between (now) and prev frame's time 
  dt = clock.sec() - clockThisFrame;
  //////dt *= speedMultiplier;  // Speedup multiplier increases the size of the time step.
  // Velocities are multiplied by dt, so things move faster when dt is bigger.
  clockThisFrame = clock.sec();

  controllerUpdate();
  
  switch( gameState ) {
  case GameState::Title:
    titleScreen->update( dt );
    break;
  
  case GameState::Running: {
      // The game could end while turboing, so we let the game end if it exits running state
      for( int i = 0; i < engineIterationPerFrame   &&   gameState == GameState::Running; i++ )
        runGame();
    }
	  break;
	
  case GameState::Paused: 
		pausedText->update( dt );
	  break;
  
  case GameState::Won:
  case GameState::Lost:
    gameOverScreen->update( dt );
    break;
  case GameState::Exiting:
	  break;
	
  case GameState::Test:
    test.update( dt );
    break;
  }
  
  controller.clearEventKeys();
}

void Game::draw() {
	sdl->clear( bkgColor );
  
	switch( gameState ) {
  case GameState::Title:
    titleScreen->draw();
    break;
    
  case GameState::Running:
    invaderGroup.draw();
		player->draw();
		
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
  case GameState::Won:
  case GameState::Lost:
    gameOverScreen->draw();
    break;
  case GameState::Exiting:
	  break;
   
  case GameState::Test:
    test.draw();
    break;  
  }
	
  if( gameState != GameState::Test ) { 
    scoreSprite->draw();  //always draw the score on top of everything else, except in test mode
  }
	sdl->present();
}

