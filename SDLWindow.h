#pragma once

#include <map>
using namespace std;

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// If you link the SDL libraries here, you don't
// have to worry about setting the project settings
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_image.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )
#endif

#include "Log.h"
#include "Vectorf.h"

// Stores everything to do with SDL, and game assets
class SDLWindow {
	// asset maps: filename=>SDL_* objects
	map<string, SDL_Texture*> texes;
	map<string, Mix_Music*> musics;
	map<string, Mix_Chunk*> sfx;

  inline static int winWidth = 640, winHeight = 480;
  
public:
	SDL_Window* window = 0;
	SDL_Renderer* renderer = 0;
  
  static void SDLInit();

	// ctor requires just window width & height
  SDLWindow(const char *title, int windowWidth, int windowHeight);
	~SDLWindow();
	
	inline Vector2f getSize(){
		return Vector2f( winWidth, winHeight );
	}
	void setColor( SDL_Color color );

	// Draws you a simple line
	void line( int startX, int startY, int endX, int endY, SDL_Color color );
	void rect( int x, int y, int w, int h, SDL_Color color );
	void fillRect( int x, int y, int w, int h, SDL_Color color );
	void drawTexture( SDL_Rect rect, SDL_Texture* tex );
	void playMusic( string musicFile );
	void playSound( string soundFile );
	
	SDL_Surface* loadSurface( string filename );
	SDL_Texture* loadTexture( string filename );
	SDL_Texture* makeText( TTF_Font* font, string text, SDL_Color color );
	Mix_Music* loadMusic( string filename );
	Mix_Chunk *loadWavSound( string waveFilename );
};
