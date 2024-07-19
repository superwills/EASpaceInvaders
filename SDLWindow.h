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
#endif

#include "Log.h"
#include "Vectorf.h"
#include "Assets.h"

// Stores everything to do with SDL, and game assets
class SDLWindow {
	// asset maps: filename=>SDL_* objects
	map<string, SDL_Texture*> texes;
	map<Music, Mix_Music*> musics;
	map<SFX, Mix_Chunk*> sfx;

  int winWidth = 640, winHeight = 480;
  
public:
	SDL_Window *window = 0;
	SDL_Renderer *renderer = 0;
  
  static void SDLInit();
  static void errorOut( const char *msg );

	// ctor requires just window width & height
  SDLWindow( const char *title, int windowWidth, int windowHeight );
	~SDLWindow();
	
	inline Vector2f getSize(){
		return Vector2f( winWidth, winHeight );
	}
	void setColor( SDL_Color color );

	// SDL supplied graphics primitives functionality
	void line( int startX, int startY, int endX, int endY, SDL_Color color );
	void rect( int x, int y, int w, int h, SDL_Color color );
	void fillRect( int x, int y, int w, int h, SDL_Color color );
	void drawTexture( SDL_Rect rect, SDL_Texture *tex );
	
	SDL_Surface* loadSurface( string filename );
	SDL_Texture* loadTexture( string filename );
	SDL_Texture* makeText( TTF_Font *font, string text, SDL_Color color );
  
	// Knowing SDLWindow is slowly becoming a god-class, in the interests of
  // simplicity we add on sound playing functionality here. SDL makes the code simple,
  // so it isn't a lot of code.
  void loadMusic( Music musicId, string filename );
  void playMusic( Music musicId );
  
  void loadWavSound( SFX sfxId, string waveFilename );
  void playSound( SFX sfxId );
};

// Give access to the SDL object using this extern.
// Alternatively could have used SDLWindow::instance()
extern SDLWindow *sdl;
