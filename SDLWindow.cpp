#include "SDLWindow.h"
#include "Sprite.h"

SDLWindow *sdl = 0;

void SDLWindow::SDLInit() {
  if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
    ExitApp( "SDL_Init failed" );
  }
  
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512 ) < 0 )  {
    string err = makeString( "Mix_OpenAudio failed: `%s`", Mix_GetError() );
    ExitApp( err.c_str() );
  }
  
  if( TTF_Init() < 0 ) {
    ExitApp( "TTF_Init failed" );
  }
  
  defaultFont = TTF_OpenFont("assets/unispace.ttf", 128);
  if( !defaultFont ) {
    ExitApp( makeString( "TTF_OpenFont: %s", TTF_GetError() ) );
  }
  
}

void SDLWindow::ExitApp( const string &msg ) {
  printf( "`%s`: SDL: `%s`\n", msg.c_str(), SDL_GetError() );
  exit( 1 );
}

SDLWindow::SDLWindow( const string &title, int windowWidth, int windowHeight ) {
	window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    windowWidth, windowHeight, SDL_WINDOW_SHOWN );
	if( !window )	{
		ExitApp( "SDL_CreateWindow failed" );
	}
	
  renderer = SDL_GetRenderer( window );
  if( renderer ) {
    info( "Destroying old renderer" );
    SDL_DestroyRenderer( renderer );
  }
  
  renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE );
  if( !renderer ) {
    ExitApp( "Could not create renderer" );
  }
	  
	// Turn on alpha blending
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
}

SDLWindow::~SDLWindow() {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	SDL_Quit();
}

Vector2f SDLWindow::getWindowSize() {
  int w, h;
  SDL_GetWindowSize( window, &w, &h );
  return Vector2f( w, h );
}

void SDLWindow::clear( SDL_Color color ) {
  setColor( color );
  SDL_RenderClear( sdl->renderer );
}

void SDLWindow::setColor( SDL_Color color ) {
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
}

void SDLWindow::present() {
  SDL_RenderPresent( renderer );
}

void SDLWindow::line( int startX, int startY, int endX, int endY, SDL_Color color ) {
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
	SDL_RenderDrawLine( renderer, startX, startY, endX, endY );
}

void SDLWindow::outlineRect( const RectF &rect, SDL_Color color ) {
	SDL_Rect sdlRect = rect.toSDLRect();
	setColor( color );
	SDL_RenderDrawRect( renderer, &sdlRect );
}

void SDLWindow::fillRect( const RectF &rect, SDL_Color color ) {
  SDL_Rect sdlRect = rect.toSDLRect();
	setColor( color );
	SDL_RenderFillRect( renderer, &sdlRect );
}

void SDLWindow::drawTexture( const RectF &rect, SDL_Texture *tex ) {
 // Convert our floating pt rect to an int-based rect
  SDL_Rect sdlRect = rect.toSDLRect();
	SDL_RenderCopy( renderer, tex, NULL, &sdlRect );
}

SDL_Surface* SDLWindow::loadSurface( const string &filename ) {
	SDL_Surface* surface = IMG_Load( filename.c_str() );
  if( !surface ) {
    error( "Couldn't load surface `%s`!", filename.c_str() );
  }
  return surface;
}

SDL_Texture* SDLWindow::loadTexture( const string &filename ) {
	auto iter = texes.find( filename );
	if( iter != texes.end() )	{
		warning( "`%s` already loaded", filename.c_str() );
		return iter->second;
	}
  
  SDL_Surface *surface = loadSurface( filename );
  if( !surface ) {
    error( "loadSurface failed" );
    return 0;
  }
  
	SDL_Texture *tex = SDL_CreateTextureFromSurface( renderer, surface );
  if( !tex ) {
    error( "`%s`: SDL_CreateTextureFromSurface failed %s", filename.c_str(), SDL_GetError() );
    return 0;
  }
  SDL_FreeSurface( surface );
  
  // default textures to allow alpha blend.
	SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
	texes[ filename ] = tex; // You can look up textures later also.
	return tex;
}

SDL_Texture* SDLWindow::makeTextTexture( const string &text, SDL_Color color ) {
	SDL_Surface *textSurface = TTF_RenderText_Solid( defaultFont, text.c_str(), color );
  if( !textSurface ) {
    error( "TTF_RenderText_Solid failed %s", text.c_str() );
    return 0;
  }
  
	SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, textSurface );
  if( !texture ) {
    error( "SDL_CreateTextureFromSurface failed %s", text.c_str() );
    return 0;
  }
	
  SDL_FreeSurface( textSurface );
	return texture;
}

void SDLWindow::loadMusic( Music musicId, const string &filename ) {
	auto iter = musics.find( musicId );
	if( iter != musics.end() ) {
		warning( "Music `%s` already loaded", filename.c_str() );
    return;
	}

	Mix_Music *music = Mix_LoadMUS( filename.c_str() ) ;
  if( !music ) {
    error( "Couldn't load music `%s`!", filename.c_str() );
  }
	musics[ musicId ] = music;
}

void SDLWindow::playMusic( Music musicId ) {
  auto it = musics.find( musicId );
  if( it == musics.end() )   {
    warning( "no music %d", musicId );
    return;
  }

  Mix_HaltMusic(); // stop all other music.
  Mix_PlayMusic( it->second, -1 );
}

void SDLWindow::loadWavSound( SFX sfxId, const string &waveFilename ) {
	auto iter = sfx.find( sfxId );
  if( iter != sfx.end() ) {
		warning( "Sound `%s` already loaded", waveFilename.c_str() );
		return;
	}

	Mix_Chunk *sound = Mix_LoadWAV( waveFilename.c_str() ) ;
  if( !sound ) {
    error( "Couldn't load sound `%s`!", waveFilename.c_str() );
  }
	sfx[ sfxId ] = sound;
}

void SDLWindow::playSound( SFX sfxId ) {
  auto it = sfx.find( sfxId );
  if( it == sfx.end() ) {
    warning( "No sound %d", sfxId );
    return;
  }

  Mix_PlayChannel( -1, it->second, 0 ) ;
}

