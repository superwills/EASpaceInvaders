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

// C++ interface to SDL functionality.
class SDLWindow {
	// asset maps: filename=>SDL_* objects
	map<string, shared_ptr<Texture> > texes;
	map<MusicId, Mix_Music*> musics;
	map<SFXId, Mix_Chunk*> sfx;
  map<AnimationId, Animation> animations;
   
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
  // Drawing textures is always done using an Animation::Frame.
  void draw( const RectF &whereToDraw, const Animation::Frame &frame );
  
  // Texture loading.
	shared_ptr<Texture> loadTexture( const string &filename );
 
  Animation getAnimation( AnimationId animationId ); 
  Animation loadSpritesheetAnimation( AnimationId animationId, const string &filename, int numFrames, const Vector2f &frameSize );
  
  // Makes a texture containing `text`, in `color` specified
  // A lot less efficient than texture atlasing, but good for prototype
	shared_ptr<Texture> makeTextTexture( const string &text, SDL_Color color );
  
  // Music/SFX
	// SDLWindow is kind of a god-class with lots of abilities,
  // but the amount of code needed is really small with SDL
  void loadMusic( MusicId musicId, const string &filename );
  void playMusic( MusicId musicId );
  
  void loadWavSound( SFXId sfxId, const string &waveFilename );
  void playSound( SFXId sfxId );
};

// Give access to the SDL object using this extern.
// Alternatively could have used SDLWindow::instance()
extern SDLWindow *sdl;
