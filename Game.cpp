#include "Game.h"

#include "Bullet.h"
#include "Bunker.h"
#include "SDLColors.h"
#include "GameOverScreen.h"
#include "Invader.h"
#include "Item.h"
#include "Particle.h"
#include "Player.h"
#include "ScoreDisplay.h"
#include "TitleScreen.h"
#include "UFO.h"

shared_ptr<Game> game;

void Game::initGameBoard() {
  Vector2f windowSize = sdl->getWindowSize();
  
  // re/create the player
  player = std::make_shared<Player>();
  updateNumberOfPlayerLives();
  
  invaderGroup.populate();
  
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
  playerLives.clear();
  playOnceAnimations.clear();
  allItems.clear();
  allScores.clear();
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

void Game::genUFO() {
  nextUFO -= dt;
  if( nextUFO < 0 ) {
    nextUFO = UFOInterval;
    
    RectF ufoBox;
    ufoBox.size = Vector2f( 64, 32 );
    ufoBox.pos.x = sdl->getWindowSize().x; // start offscreen right
    ufoBox.pos.x--; // 1 pixel in to be sure it doesn't immediately die for being offscreen
    ufoBox.pos.y = ufoBox.size.y / 2;
    shared_ptr<UFO> ufo = std::make_shared<UFO>( ufoBox );
    allUFOs.push_back( ufo );
  }
}

int Game::getNumBullets( bool isFromInvader ) {
  int num = 0;
  for( auto bullet : allBullets ) {
    if( isFromInvader == bullet->isFromInvader() ) {
      num++;
    }
  }
  return num;
}

void Game::checkWinConditions() {
  // player won if all invaders are gone
  if( invaderGroup.empty() ) {
    info( "Player won" );
    setState( GameState::Won );
    gameOverScreen->win();
  }
  
  // See if the invaders won by reaching the bottom, or player dead and death animation finished.
  ///if( invaderGroup.didInvadersWin() || (player->dead && !player->canPlayerRespawn() ) ) {
  if( invaderGroup.didInvadersWin() || (player->dead && player->animation.isEnded() && player->isOutOfLives() ) ) {
    info( "Invaders won" );
    setState( GameState::Lost );
    gameOverScreen->lose();
  } 
}

void Game::checkBulletCollisions() {
  for( auto bullet : allBullets ) {
    // All bullet check bunker
    for( auto bunker : allBunkers ) {
      if( bunker->killHit( bullet ) ) {
        bullet->die();
        break;
      }
    }
    
    if( bullet->dead ) {
      continue;
    }
  
    if( bullet->isFromInvader() ) {
      // Invader bullet check against player.
      if( !player->dead && bullet->hit( player ) ) {
        bullet->die();
        
        // if the player is shielded he just loses his shields
        if( player->shielded ) {
          player->loseShield();
        }
        else {
          killPlayer();
        }
      }
      
      // If it missed the player, we should skip the checks for other invaders + ufos below
      continue;  // there's nothing else for an invader bullet to do
    }
    else { // is Player bullet
      // check invaders
      for( auto invader : invaderGroup.invaders ) {
        if( invader->dead ) {
          continue; // can happen if 2 player bullets try to hit same invader
        }
        
        if( bullet->hit( invader ) ) {
          if( bullet->type != BulletType::PlayerThickLaser ) {
            // the laser doesn't die
            bullet->die();
          }
          invader->die();
          break;
        }
      }
      
      if( bullet->dead ) {
        // If the bullet was consumed by an invader, skip the ufo check that would happen below
        continue;
      }
      
      // Player bullet check UFO
      for( auto ufo : allUFOs ) {
        if( bullet->hit( ufo ) ) {
          bullet->die();
          ufo->die();
        }
      }
    }
  }
}

void Game::checkAllCollisions() {
	// check bullets against invaders, ufo's, bunkers.
  checkBulletCollisions();
  
  // Check invaders against player itself, bunkers
  for( auto invader : invaderGroup.invaders ) {
    if( !player->dead && invader->hit( player ) ) {
      killPlayer();
    }
    for( auto bunker : allBunkers ) {
      // kill bunker pieces hit by this invader
      bunker->killHit( invader );
    }
  }
  
  for( auto item : allItems ) {
    if( player->hit( item ) ) {
      sdl->playSound( SFXId::Blip );
      player->giveItem( item );
      item->die();
    }
  }
}

void Game::clearDead() {
  clearDeadOnes( allBullets );
  invaderGroup.clearDead();
  clearDeadOnes( allParticles );
  clearDeadOnes( allUFOs );
  for( auto bunker : allBunkers ) {
    bunker->clearDead();
  }
  clearDeadOnes( playOnceAnimations );
  clearDeadOnes( allItems );
  clearDeadOnes( allScores );
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
	
  for( auto playerLife : playerLives ) {
    playerLife->update( dt );
  } 
  
  for( auto sprite : playOnceAnimations ) {
    sprite->update( dt );
    
    if( sprite->animation.isEnded() ) {
      sprite->die();
    }
  }
  
  for( auto item : allItems ) {
    item->update( dt );
  }
  
  for( auto score : allScores ) {
    score->update( dt );
  }
  
  checkAllCollisions();
  
  checkWinConditions();
  
  clearDead(); 
}

void Game::killPlayer() {
  // call the player to die.
  player->die();
  updateNumberOfPlayerLives();
  
  // Kill all powerups on screen
  for( auto item : allItems ) {
    item->die();
  }
}

void Game::updateNumberOfPlayerLives() {
  playerLives.clear();
  
  float lifeBoxSize = sdl->getWindowSize().x * .02;
  RectF nextLifeBox( 0, 0, lifeBoxSize, lifeBoxSize );
  for( int i = 0; i < player->getNumLivesRemaining(); i++ ) {
    nextLifeBox.pos.x += lifeBoxSize;
    playerLives.push_back( std::make_shared<Sprite>( nextLifeBox, AnimationId::Player ) );
  }
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

void Game::init() {

  bkgColor = Indigo;
  
  // Load sprite animations
  sdl->loadSpritesheetAnimation( AnimationId::Invader1, "assets/ims/invader-1.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Invader2, "assets/ims/invader-2.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::InvaderE, "assets/ims/invader-E.png", 4, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::InvaderA, "assets/ims/invader-A.png", 4, Vector2f( 16, 16 ) );
  
  sdl->loadSpritesheetAnimation( AnimationId::BulletInvaderArrow, "assets/ims/bullet-arrow.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletInvaderLightning, "assets/ims/bullet-lightning.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletPlayerArrow, "assets/ims/bullet-player.png", 2, Vector2f( 8, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletPlayerBall, "assets/ims/bullet-ball.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::BulletLaser, "assets/ims/bullet-thick-laser.png", 2, Vector2f( 8, 16 ) );
  
  sdl->loadSpritesheetAnimation( AnimationId::Explode, "assets/ims/explode.png", 5, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::MenuPointer, "assets/ims/menu-pointer.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::UFO, "assets/ims/ufo.png", 2, Vector2f( 32, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Player, "assets/ims/player.png", 1, Vector2f( 16, 8 ) );
  sdl->loadSpritesheetAnimation( AnimationId::PlayerDie, "assets/ims/player-die.png", 6, Vector2f( 20, 12 ), White, 0 );
  sdl->loadSpritesheetAnimation( AnimationId::Boss, "assets/ims/josh.png", 2, Vector2f( 64, 32 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Shield, "assets/ims/shields.png", 4, Vector2f( 16, 16 ) );
  
  sdl->loadSpritesheetAnimation( AnimationId::ItemPlus1, "assets/ims/item+1.png", 6, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::ItemSpread, "assets/ims/item-spread.png", 6, Vector2f( 17, 17 ) );
  sdl->loadSpritesheetAnimation( AnimationId::ItemThickLaser, "assets/ims/item-thick-laser.png", 6, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::ItemShield, "assets/ims/item-shield.png", 6, Vector2f( 16, 16 ) );
  
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
  sdl->loadWavSound( SFXId::UFO, "assets/sounds/ufo-0.wav" ); 
  
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

void Game::tryShootBullet( BulletType bulletType, const Vector2f &shootPoint ) {

  bool isFromInvader = Bullet::IsBulletTypeFromInvader( bulletType );
  int numBullets = getNumBullets( isFromInvader );
  int maxBullets = isFromInvader ? invaderGroup.getMaxNumBullets() : player->getMaxBullets(); 
  if( numBullets >= maxBullets ) {
    // can't shoot because max #bullets for type is reached.
    return;
  }
  
  if( bulletType == BulletType::PlayerSpread ) {
    // Spread makes __3__ bullets
    for( int i = -1; i <= 1; i++ ) {
      auto bullet = std::make_shared<Bullet>( shootPoint, bulletType );
      bullet->velocity.x = i * 50 ;
      allBullets.push_back( bullet );
    }
  }
  else {
    auto bullet = std::make_shared<Bullet>( shootPoint, bulletType );
    allBullets.push_back( bullet );
  }
  
}

void Game::playSpriteAnimation( const RectF &where, AnimationId animationId ) {
  auto spriteAnim = std::make_shared<Sprite>( where, animationId );
  spriteAnim->animation.cycles = 0;  // DO NOT cycle the anim, so we can remove it when it's done playing thru
  playOnceAnimations.push_back( spriteAnim );
}

void Game::particleSplash( const Vector2f &pos, int numParticles, float sizeMin, float sizeMax ) {
  for( int i = 0; i < numParticles; i++ ) {
    Vector2f size = Vector2f::random( sizeMin, sizeMax );
    shared_ptr<Particle> p = std::make_shared<Particle>( RectF( pos, Vector2f( size ) ) );
    allParticles.push_back( p );
  }
}

void Game::createItem( const Vector2f &pos, AnimationId animationId ) {
  auto item = std::make_shared<Item>( pos, animationId );
  allItems.push_back( item );
}

void Game::displayScore( int score, const Vector2f &pos, SDL_Color color ) {
  // Create the score sprite
  auto scoreSprite = std::make_shared<ScoreDisplay>( score, pos, White );
  allScores.push_back( scoreSprite );
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
    
    for( auto playerLife : playerLives ) {
      playerLife->draw();
    }
    
    for( auto sprite : playOnceAnimations ) {
      sprite->draw();
    }
    
    for( auto item : allItems ) {
      item->draw();
    }
    
    for( auto score : allScores ) {
      score->draw();
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

