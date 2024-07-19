#pragma once

#include "SDLWindow.h"
#include "Sprite.h"
#include "Paddle.h"
#include "Ball.h"
#include "Controller.h"
#include "TitleScreen.h"

#include <memory>
using std::shared_ptr;

class Game {
public:
	enum GameState { Title, Running, JustScored, Paused, Exiting };
	
private:
	// Need a ball and paddles for this game
	Ball* ball;
	Paddle* leftPaddle;
	Paddle* rightPaddle;

	TitleScreen* title;
	Sprite* pausedText;
	SDL_Color bkgColor;	// the current background color
	int flashesRem;

	// Keeping track of scores
	int leftScoreValue = 0, rightScoreValue = 0;
	Sprite *leftScoreSprite = 0, *rightScoreSprite = 0;
	
	TTF_Font *font;
	
  // For unpausing
	GameState prevState;
	GameState gameState;
	
	Controller controller;

public:
	Game();
	~Game();
	void leftPlayerScored();
	void rightPlayerScored();
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
