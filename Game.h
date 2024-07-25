#pragma once

#include "BulletType.h"
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
class Item;
class Particle;
class Player;
class ScoreDisplay;
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
  vector< shared_ptr<Sprite> > playerLives;
  vector< shared_ptr<Sprite> > playOnceAnimations;
  vector< shared_ptr<Item> > allItems;
  vector< shared_ptr<ScoreDisplay> > allScores;
  
  InvaderGroup invaderGroup;
  Test test;
  
	SDL_Color bkgColor;	// the current background color
  
	int score = 0;
  shared_ptr<Sprite> scoreSprite;
  
  // You get a UFO every (interval) sec.
  inline static float UFOInterval = 20;
  float nextUFO = UFOInterval;
	
	GameState prevState = GameState::Title;
  GameState gameState = GameState::Title;
  
  float shake = 0;
  
  vector<uint16_t> startKeys = { SDL_SCANCODE_RETURN, SDL_SCANCODE_RETURN2, SDL_SCANCODE_KP_ENTER };
  Controller controller;
  
  void initGameBoard();
  void clearGameBoard();
  
  void togglePause();
  void genUFO();
  
  int getNumBullets( bool isFromInvader );
  
  void checkWinConditions();
  
  void checkBulletCollisions();
  void checkAllCollisions();
  
  void clearDead(); 
	void runGame();
  void killPlayer();
  void updateNumberOfPlayerLives();
  
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
  void tryShootBullet( BulletType bulletType, const Vector2f &shootPoint );
  // Plays a sprite animation ONCE where you want it
  void playSpriteAnimation( const RectF &where, AnimationId animationId );
  
  // You can start the particle decayed, so that it doesn't start at 100% alpha with initialDecay
  void particleSplash( const Vector2f &pos, int numParticles, float sizeMin, float sizeMax, float initialDecay = 1 );
  void particleCloud( const RectF &box, int numParticles, float sizeMin, float sizeMax, float initialDecay = 1 );
  void createItem( const Vector2f &pos, AnimationId animationId );
  
  void displayScore( int score, const Vector2f &pos, SDL_Color color );
  void setScore( int newScore );
  void changeScore( int byScoreValue );
  
  void update();
	void draw();
};

extern shared_ptr<Game> game;
