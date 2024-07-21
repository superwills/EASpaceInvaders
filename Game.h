#pragma once

#include "Controller.h"
#include "InvaderGroup.h"
#include "SDLWindow.h"
#include "Sprite.h"
#include "StopWatch.h"
#include "TitleScreen.h"

#include <memory>
#include <vector>
using std::shared_ptr;

class Bullet;
class Invader;
class InvaderGroup;
class Particle;
class Player;
class UFO;

class Game {
public:
	enum class GameState { Title, Running, Paused, Won, Lost, Exiting };
  float clockThisFrame = 0, dt = 0;
  StopWatch clock;
  
private:
  shared_ptr<TitleScreen> title;
	shared_ptr<Sprite> pausedText;
  shared_ptr<Player> player;
  
  vector< shared_ptr<Bullet> > allBullets;
  vector< shared_ptr<Particle> > allParticles;
  vector< shared_ptr<UFO> > allUFOs;
  
  InvaderGroup invaderGroup;
  
	SDL_Color bkgColor;	// the current background color
  
	int score = 0;
  shared_ptr<Sprite> scoreSprite;
  
  // You get a UFO every (interval) sec.
  inline static float UFOInterval = 10;
  float nextUFO = UFOInterval;
	
	GameState prevState = GameState::Title;
  GameState gameState = GameState::Title;

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
  void genUFO();
  void particleSplash( const Vector2f &pos, int numParticles );
  void changeScore( int byScoreValue );
	void checkForCollisions();
  void clearDead(); 
	void runGame();
 
  void controllerUpdate(); 
	void update();
	void draw();
};

extern Game *game;
