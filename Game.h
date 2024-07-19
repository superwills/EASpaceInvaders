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
	enum GameState { Title, Running, JustScored, Paused, Exiting };
	
private:
  vector<Invader*> invaders;
	// Need a ball and paddles for this game
	Ball* ball;
	Player* player;

	TitleScreen* title;
	Sprite* pausedText;
	SDL_Color bkgColor;	// the current background color
	int flashesRem;

	// Keeping track of scores
	int leftScoreValue = 0, rightScoreValue = 0;
	Sprite *leftScoreSprite = 0, *rightScoreSprite = 0;
	
	// For unpausing
	GameState prevState, gameState;
	
	Controller controller;

public:
	Game();
  ~Game() { }
	void leftPlayerScored();
	void rightPlayerScored();
  
  bool isState( GameState state );
	GameState getState();
	void setState( GameState newState );
	void togglePause();
	void resetBall();
	void drawScores();
	void checkForCollisions();
	void runGame();
	void update();
	void draw();
};

extern Game* game;
