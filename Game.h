#pragma once

#include "Controller.h"
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
public:
	enum class GameState { Title, Running, Paused, Won, Lost, Exiting, Test };
  float clockThisFrame = 0, dt = 0;
  float speedMultiplier = 1;
  StopWatch clock;
  
private:
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

  vector<uint16_t> startKeys = { SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN, SDL_SCANCODE_RETURN2, SDL_SCANCODE_KP_ENTER };
  
public:
  Controller controller;

 	Game() { }
  ~Game() { }
	
  void init(); 
  bool isState( GameState state );
	GameState getState();
	void setState( GameState newState );
	void togglePause();
  
  void initGameBoard();
  void clearGameBoard();
  void genUFO();
  
  // Player shoots from top, invaders from their bottom.
  void shootBullet( const RectF &source, bool fromInvader, const Vector2f &vel );
  void particleSplash( const Vector2f &pos, int numParticles );
  void setScore( int newScore );
  void changeScore( int byScoreValue );
  
  void checkWinConditions();
	void checkForCollisions();
  void clearDead(); 
	void runGame();
 
  void controllerUpdateTitle();
  void controllerUpdateRunning();
  void controllerUpdate(); 
	void update();
	void draw();
};

extern shared_ptr<Game> game;
