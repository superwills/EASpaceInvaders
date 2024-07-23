#pragma once

#include "Controller.h"
#include "GameState.h"
#include "InvaderGroup.h"
#include "SDLWindow.h"
#include "Sprite.h"
#include "StopWatch.h"
#include "Test.h"

#include <memory>
#include <vector>
using std::shared_ptr;

class Bullet;
class Bunker;
class GameOverScreen;
class Invader;
class InvaderGroup;
class Particle;
class Player;
class TitleScreen;
class UFO;

class Game {
  float clockThisFrame = 0, dt = 0;
  StopWatch clock;
  int engineIterationPerFrame = 1;  // process multiple game steps / display interval
	
  shared_ptr<TitleScreen> titleScreen;
  shared_ptr<GameOverScreen> gameOverScreen;
  
	shared_ptr<Sprite> pausedText;
  shared_ptr<Player> player;
  
  vector< shared_ptr<Bullet> > allBullets;
  vector< shared_ptr<Bunker> > allBunkers;
  vector< shared_ptr<Particle> > allParticles;
  vector< shared_ptr<UFO> > allUFOs;
  
  InvaderGroup invaderGroup;
  Test test;
  
	SDL_Color bkgColor;	// the current background color
  
	int score = 0;
  shared_ptr<Sprite> scoreSprite;
  
  // You get a UFO every (interval) sec.
  inline static float UFOInterval = 10;
  float nextUFO = UFOInterval;
	
	GameState prevState = GameState::Title;
  GameState gameState = GameState::Title;
  
  // Invaders fit in a box this% of the original window size.
  inline static const Vector2f DefaultInvaderBoundsSizePercent = Vector2f( .5 );

  vector<uint16_t> startKeys = { SDL_SCANCODE_RETURN, SDL_SCANCODE_RETURN2, SDL_SCANCODE_KP_ENTER };
  Controller controller;
  
  void initGameBoard();
  void clearGameBoard();
  
  void togglePause();
  void genUFO();
  
  int getNumBullets( bool isFromInvader );
  
  void checkWinConditions();
	void checkForCollisions();
  void clearDead(); 
	void runGame();
 
  void controllerUpdateTitle();
  void controllerUpdateRunning();
  void controllerUpdate(); 
  
public:
 	Game() { }
  ~Game() { }
	
  void init(); 
  
  inline bool isState( GameState state ) { return gameState == state; }
	void setState( GameState newState );
	
  void setKeyJustPressed( uint16_t key );
  void setMouseJustClicked( uint16_t mouseButton );
   
  // Player shoots from top, invaders from their bottom.
  void tryShootBullet( const RectF &source, bool isFromInvader, const Vector2f &vel );
  void particleSplash( const Vector2f &pos, int numParticles, float sizeMin, float sizeMax );
  void setScore( int newScore );
  void changeScore( int byScoreValue );
  
  void update();
	void draw();
};

extern shared_ptr<Game> game;
