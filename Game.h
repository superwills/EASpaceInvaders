#pragma once

#include "SDLWindow.h"
#include "Sprite.h"
#include "Ball.h"
#include "Controller.h"
#include "TitleScreen.h"

#include <memory>
#include <vector>
using std::shared_ptr;

class Invader;
class Player;

class Game {
public:
	enum class GameState { Title, Running, Paused, Won, Lost, Exiting };
	
private:
  vector<Invader*> invaders;
	// Need a ball and paddles for this game
	Ball* ball;
	Player* player;

	TitleScreen* title;
	Sprite* pausedText;
	SDL_Color bkgColor;	// the current background color
  int flashesRem = 0;
  
	int score = 0;
  Sprite *scoreSprite = 0;
	
	// For unpausing
	GameState prevState, gameState;
	
	Controller controller;

public:
	Game();
  ~Game() { }
	
  bool isState( GameState state );
	GameState getState();
	void setState( GameState newState );
	void togglePause();
  
  void changeScore( int byScoreValue );
	void resetBall();
	void checkForCollisions();
	void runGame();
	void update();
	void draw();
};

extern Game* game;
