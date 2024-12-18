#include "Game.h"

#include "Bullet.h"
#include "BulletLaser.h"
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

Game::Game( const RectF &bounds ) :
    quadtree( bounds ) {
}

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
    nextUFO = UFO::GenInterval;
    
    shared_ptr<UFO> ufo = std::make_shared<UFO>();
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
  if( invaderGroup.didInvadersWin() || (player->isDead() && player->animation.isEnded() && player->isOutOfLives() ) ) {
    info( "Invaders won" );
    setState( GameState::Lost );
    gameOverScreen->lose();
  } 
}

void Game::checkBulletCollisions_basic() {
  for( auto bullet : allBullets ) {
    if( bullet->isDead() )  skip;
    
    // All bullet check bunker
    for( auto bunker : allBunkers ) {
      if( bunker->isDead() )  skip;
      
      if( bunker->hit( bullet ) ) {
        if( bullet->isDead() ) {
          break;
        }
      }
    }
    
    // bullet impacted a bunker? it may have died, but it won't if it was a laser
    if( bullet->isDead() )  skip;
    
    // Check bullet-bullet
    for( auto oBullet : allBullets ) {
      if( oBullet->isDead() )  skip;
      
      // from differing teams?
      if( bullet->isFromInvader() != oBullet->isFromInvader() ) {
        if( bullet->hit( oBullet ) ) {
          if( bullet->isDead() ) {
            break; // if the bullet wasn't a laser, it will die on impact, so skip other bullet-bullet checks
          }
        }
      }
    }
    
    if( bullet->isDead() )  skip;
    
    // Invader bullet check against player.
    if( bullet->isFromInvader() ) {
      if( !player->isDead() ) {
        if( bullet->hit( player ) ) {
          
        }
      }
    }
    else { // is Player bullet
      // Player bullet checks Invaders + UFOs (but Invader bullets don't)
      for( auto invader : invaderGroup.invaders ) {
        // can happen if 2 player bullets try to hit same invader
        if( invader->isDead() )  skip;
        
        if( bullet->hit( invader ) ) {
          if( bullet->isDead() ) {
            break;
          }
        }
      }
      
      // If the bullet was consumed by an invader, skip the ufo check that would happen below
      if( bullet->isDead() )  skip;
      
      // Player bullet check UFO
      for( auto ufo : allUFOs ) {
        if( bullet->hit( ufo ) ) {
        }
      }
    }
  }
}

void Game::checkAllCollisions_basic() {
  RectF::BoxHits = 0;
	// check bullets against invaders, ufo's, bunkers.
  checkBulletCollisions_basic();
  
  // Check invaders against player itself, bunkers
  for( auto invader : invaderGroup.invaders ) {
    if( invader->isDead() )  skip;
    
    if( !player->isDead() ) {
      if( invader->hit( player ) ) {
        // game will be over.
        return;
      }
    }
    for( auto bunker : allBunkers ) {
      if( bunker->isDead() )  skip;
      
      if( bunker->hit( invader ) ) {
        // usually can't hit more than 1 bunker because they're far apart
        // keep checking other bunkers too
      }
    }
  }
  
  for( auto item : allItems ) {
    if( item->isDead() )  skip;
    
    if( player->hit( item ) ) {
      // player takes item in Player::onHit
    }
  }
  
  debugText = makeString( "%d", RectF::BoxHits );
}

void Game::buildQuadtree() {
  RectF::BoxHits = 0;
  quadtree = Quadtree( sdl->getWindowRectangle() );
  quadtree.add( player );
  for( auto bullet : allBullets ) {
    quadtree.add( bullet );
  }
  for( auto bunker : allBunkers ) {
    quadtree.add( bunker );
  }
  // Notice how we do not add the bunkerPieces because they are in the bunkers
  for( auto ufo : allUFOs ) {
    quadtree.add( ufo );
  }
  for( auto item : allItems ) {
    quadtree.add( item );
  }
  for( auto invader : invaderGroup.invaders ) {
    quadtree.add( invader );
  }
  debugText = makeString( "%d", RectF::BoxHits );
}

void Game::checkAllCollisions_quadtree() {
  buildQuadtree();
  
  RectF::BoxHits = 0;
  // Now run collisions of each intersectable against a quadtree query
  vector< SP_ICollideable > candidates;
  
  if( !player->isDead() ) {
    candidates = quadtree.query( player );
    for( auto cand : candidates ) {
      if( player == cand || cand->isDead() )  skip;
      
      if( player->hit( cand ) ) {
        if( player->isDead() ) {
          break;
        }
      }
    }
  }
  
  for( auto bullet : allBullets ) {
    if( bullet->isDead() )  skip;
    
    candidates = quadtree.query( bullet );
    
    for( auto cand : candidates ) {
      // Quadtree always pulls self
      if( bullet == cand || cand->isDead() )  skip;
      
      // Bullets can pretty much hit anything, so check against all
      if( bullet->hit( cand ) ) {
        if( bullet->isDead() ) {
          break;
        }
      }
    }
  }
  
  for( auto invader : invaderGroup.invaders ) {
    if( invader->isDead() )  skip;
    
    candidates = quadtree.query( invader );
    
    for( auto cand : candidates ) {
      if( invader == cand || cand->isDead() )  skip;
      
      // Only check for invader-bunker & invader-player collisions
      if( isAnyOf( cand->collisionType, { ICollideableType::Bunker, ICollideableType::Player } ) ) {
        if( invader->hit( cand ) ) {
          if( invader->isDead() ) {
            break;
          }
        } 
      }
    }
  }
  
  // We don't need to query from bunkers, ufos, or items, since player, Bullets and Invaders query instead 
  debugText += makeString( "/%d", RectF::BoxHits );
  
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
  clearDeadOnes( allBunkers );
}

void Game::runGame() {
  
  // Action that pauses when player is in a dead state
  if( !player->isDead() ) {
    invaderGroup.update( dt );
    for( auto bullet : allBullets ) {
      bullet->update( dt );
    }
    genUFO();
  }
  
  player->update( dt );
  scoreSprite->update( dt );
  if( debugTextSprite )  debugTextSprite->update( dt );

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
  
  clearDead();
  
  checkAllCollisions_basic();
  //checkAllCollisions_quadtree();
  
  Vector2f windowSize = sdl->getWindowSize();
  RectF debugTextBox( windowSize.x*.85, windowSize.y*.05, windowSize.x*.1, windowSize.y*.1 );
  debugTextSprite = Sprite::Text( debugText, debugTextBox, White );
  
  checkWinConditions();
  
  clearDead();
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
  if( player->isDead() ) {
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
  Animation& thickLaser = sdl->loadSpritesheetAnimation( AnimationId::BulletLaser, "assets/ims/bullet-laser-no-outline.png", 2, Vector2f( 8, 16 ) );
  int thickLaserInitNumFrames = thickLaser.frames.size();
  for( int i = 0; i < thickLaserInitNumFrames; i++ ) {
    Animation::Frame &frame = thickLaser.frames[ i ];

    // add color banded 
    frame.duration = .02;
    frame.color = SDL_RandomSolidColor();
    Animation::Frame frameCopy = frame;
    frameCopy.color = SDL_RandomSolidColor();
    for( int i = 0; i < 15; i++ ) {
      thickLaser.addFrame( frameCopy );
    }
  }
  
  sdl->loadSpritesheetAnimation( AnimationId::Explode, "assets/ims/explode.png", 5, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::MenuPointer, "assets/ims/menu-pointer.png", 2, Vector2f( 16, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::UFO, "assets/ims/ufo.png", 2, Vector2f( 32, 16 ) );
  sdl->loadSpritesheetAnimation( AnimationId::Player, "assets/ims/player.png", 1, Vector2f( 16, 8 ) );
  Animation& playerDeath = sdl->loadSpritesheetAnimation( AnimationId::PlayerDie, "assets/ims/player-die.png", 6, Vector2f( 20, 12 ), White, 0 );
  for( auto &frame : playerDeath.frames ) {
    frame.duration *= 3;
  }
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
  sdl->loadMusic( MusicId::DemoMusic, "assets/sounds/demomusic.mp3" );
  sdl->loadMusic( MusicId::Stardust, "assets/sounds/stardust.mp3" );
  
	setState( GameState::Title );
  setScore( 0 );
}

void Game::setState( GameState newState ) {
	prevState = gameState;
	
  switch( newState ) {
  case GameState::Title:
		sdl->playMusic( rand<MusicId>( MusicId::DemoMusic, MusicId::Stardust ) );
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

void Game::playerDie() {
  updateNumberOfPlayerLives();
  
  // Kill all powerups on screen
  for( auto item : allItems ) {
    if( !item->isDead() ) {
      item->die();
    }
  }
  
  // so the player doesn't get hit when respawn
  for( auto bullet : allBullets ) {
    if( !bullet->isDead() ) {
      bullet->die();
    }
  }
  
  for( auto ufo : allUFOs ) {
    ufo->velocity *= 4; // he's leaving
  }
  
  shakeScreen( 1 );
}

void Game::shakeScreen( float shakeTime ) {
  shakeTimeRemaining += shakeTime;
  shakeTimeRemaining = std::clamp( shakeTimeRemaining, 0.f, 1.f );
}

void Game::tryShootBullet( BulletType bulletType, const Vector2f &shootPoint ) {

  bool isFromInvader = Bullet::IsBulletTypeFromInvader( bulletType );
  int numBullets = getNumBullets( isFromInvader );
  int maxBullets = isFromInvader ? invaderGroup.getMaxNumBullets() : player->getMaxBullets(); 
  if( numBullets >= maxBullets ) {
    // can't shoot because max #bullets for type is reached.
    return;
  }
  
  if( bulletType == BulletType::PlayerThickLaser ) {
    auto bullet = std::make_shared<BulletLaser>( shootPoint );
    allBullets.push_back( bullet );
  }
  else if( bulletType == BulletType::PlayerSpread ) {
    // Spread makes __3__ bullets
    for( int i = -1; i <= 1; i++ ) {
      auto bullet = std::make_shared<Bullet>( shootPoint, bulletType );
      bullet->velocity.x = i * 50;
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
  spriteAnim->particleCloudProperties.setNumParticles( 0, 0 ); // play-once anims don't need a splash
  playOnceAnimations.push_back( spriteAnim );
}

void Game::particleCloud( const RectF &insideBoxArea, const ParticleCloudProperties &particleCloudProperties ) {
  int numParticles = randIntIn( particleCloudProperties.number.min, particleCloudProperties.number.max );
  for( int i = 0; i < numParticles; i++ ) {
    RectF particleBox;
    particleBox.size = Vector2f::random( particleCloudProperties.size.min, particleCloudProperties.size.max );
    Vector2f particleCenter = insideBoxArea.randomPoint();
    particleBox.setCenter( particleCenter );
    shared_ptr<Particle> p = std::make_shared<Particle>( particleBox );
    p->lifeRemaining *= particleCloudProperties.initialDecay;
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
  clockThisFrame = clock.sec();
  
  controllerUpdate();
  
  shakeTimeRemaining -= dt;
  shakeTimeRemaining = std::clamp( shakeTimeRemaining, 0.f, 1.f );
  
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
    checkAllCollisions_quadtree(); // draw debug   
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
    
  case GameState::Paused: 
		pausedText->draw();
	  [[ fallthrough ]];
  
  case GameState::Running:
    invaderGroup.draw();
		player->draw();
    debugTextSprite->draw();

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
  
  // Apply shake effect
  RectF windowRect = sdl->getWindowRectangle();
  windowRect.pos += Vector2f::random( 0, ShakeMagnitude )*shakeTimeRemaining;
  sdl->setViewport( windowRect.toSDLRect() );
  
  drawDebug();
  
  sdl->present();
}

void Game::drawDebug() {
  //quadtree.draw();
  
  for( ColorRect &crect : debugRects ) {
    sdl->rect( crect );
    crect.update( dt );
  }
  debugRects.erase( std::remove_if( debugRects.begin(), debugRects.end(), []( const ColorRect &r ) {
    return r.isDead();
  }), debugRects.end() );
  
  
  #if 0
  quadtree.each( []( QuadtreeNode *qtNode ) {
    RectF textRect = qtNode->bounds;
    textRect.size = Vector2f( 25, 25 );
    textRect.setCenter( qtNode->bounds.mid() );
    auto spText = Sprite::Text( makeString( "%zu", qtNode->objects.size() ), textRect, White );
    game->debugText.push_back( spText );
  } );
  
  for( auto spText : debugText ) {
    spText->draw();
  }
  debugText.clear();
  #endif
}

void Game::addDebugRect( const RectF &rect, SDL_Color color, float lifetime ) {
  debugRects.emplace_back( ColorRect( rect, color, lifetime ) );
}
  
