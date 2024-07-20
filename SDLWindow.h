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

#include "Animation.h"
#include "Assets.h"
#include "Log.h"
#include "RectF.h"
#include "Vectorf.h"

// Stores everything to do with SDL, and game assets
class SDLWindow {
	// asset maps: filename=>SDL_* objects
	map<string, SDL_Texture*> texes;
	map<Music, Mix_Music*> musics;
	map<SFX, Mix_Chunk*> sfx;
  SDL_Window *window = 0;
	SDL_Renderer *renderer = 0;
  inline static TTF_Font *defaultFont = 0; // default rendering font.
  
public:
  
  static void SDLInit();
  static void ExitApp( const string &msg );

	// ctor requires just window width & height
  SDLWindow( const string &title, int windowWidth, int windowHeight );
	~SDLWindow();
	
  // Not cached, get it from the physical window
  Vector2f getWindowSize();
  void clear( SDL_Color color );
	void setColor( SDL_Color color );
  // Finished rendering the frame 
  void present(); 

	// SDL supplied graphics primitives functionality
	void line( int startX, int startY, int endX, int endY, SDL_Color color );
	void outlineRect( const RectF &rect, SDL_Color color );
	void fillRect( const RectF &rect, SDL_Color color );
	void drawTexture( const RectF &rect, SDL_Texture *tex );
	void draw( const RectF &rect, const Animation::Frame &animationFrame ); 
	
	SDL_Surface* loadSurface( const string &filename );
	SDL_Texture* loadTexture( const string &filename );
  
  // Makes a texture containing `text`, in `color` specified
  // A lot less efficient than texture atlasing, but good for prototype
	SDL_Texture* makeTextTexture( const string &text, SDL_Color color );
  
	// Knowing SDLWindow is slowly becoming a god-class, in the interests of
  // simplicity we add on sound playing functionality here. SDL makes the code simple,
  // so it isn't a lot of code.
  void loadMusic( Music musicId, const string &filename );
  void playMusic( Music musicId );
  
  void loadWavSound( SFX sfxId, const string &waveFilename );
  void playSound( SFX sfxId );
};

// Give access to the SDL object using this extern.
// Alternatively could have used SDLWindow::instance()
extern SDLWindow *sdl;
