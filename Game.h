#pragma once

#include "Controller.h"
#include "InvaderGroup.h"
#include "SDLWindow.h"
#include "Sprite.h"
#include "TitleScreen.h"

#include <memory>
#include <vector>
using std::shared_ptr;

class Bullet;
class Invader;
class InvaderGroup;
class Player;

class Game {
public:
	enum class GameState { Title, Running, Paused, Won, Lost, Exiting };
	 
private:
  shared_ptr<TitleScreen> title;
	shared_ptr<Sprite> pausedText;
  shared_ptr<Player> player;
  
  vector< shared_ptr<Bullet> > allBullets;
  
  InvaderGroup invaderGroup;
  vector< shared_ptr<Invader> > allInvaders;
  
	SDL_Color bkgColor;	// the current background color
  
	int score = 0;
  shared_ptr<Sprite> scoreSprite;
	
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
  void populateInvaders();
  void changeScore( int byScoreValue );
	void checkForCollisions();
  void clearDead(); 
	void runGame();
 
  void controllerUpdate(); 
	void update();
	void draw();
};

extern Game *game;
