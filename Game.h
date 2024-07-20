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
	
  //vector< shared_ptr<Sprite> > allSprites;
  vector< Sprite* > allSprites;
   
private:
  TitleScreen *title;
	Sprite *pausedText;
	
  Player *player;
  vector<Invader*> invaders;
  
	SDL_Color bkgColor;	// the current background color
  
	int score = 0;
  Sprite *scoreSprite = 0;
	
	GameState prevState, gameState;
	
	Controller controller;

public:
	Game();
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
	void runGame();
	void update();
	void draw();
};

extern Game* game;
