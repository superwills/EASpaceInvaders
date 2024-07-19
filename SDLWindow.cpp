#include "SDLWindow.h"
#include "Sprite.h"

void SDLWindow::SDLInit() {
  if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
    errorOut( "SDL_Init failed" );
  }
  
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 512 ) < 0 )  {
    string err = makeString( "Mix_OpenAudio failed: `%s`", Mix_GetError() );
    errorOut( err.c_str() );
  }
  
  if( TTF_Init() < 0 ) {
    errorOut( "TTF_Init failed" );
  }
}

void SDLWindow::errorOut( const char* msg ) {
  printf( "`%s`: SDL: `%s`\n", msg, SDL_GetError() );
  exit( 1 );
}

SDLWindow::SDLWindow(const char* title, int windowWidth, int windowHeight) :
    winWidth( windowWidth ), winHeight( windowHeight ) {
	Sprite::sdl = this; //BAD.
  
	window = SDL_CreateWindow( title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    winWidth, winHeight, SDL_WINDOW_SHOWN );
	if( !window )	{
		errorOut( "SDL_CreateWindow failed" );
	}
	
  renderer = SDL_GetRenderer( window );
  if( renderer ) {
    // delete default to get one with vsync on
    info( "Destroying old renderer" );
    SDL_DestroyRenderer( renderer );
  }
  
  renderer = SDL_CreateRenderer( window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE );
  if( !renderer ) {
    errorOut( "Could not create renderer" );
  }
	  
	// Turn on alpha blending
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );
}

SDLWindow::~SDLWindow() {
	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );

	SDL_Quit();
}

void SDLWindow::setColor( SDL_Color color ) {
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
}

void SDLWindow::line( int startX, int startY, int endX, int endY, SDL_Color color ) {
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
	SDL_RenderDrawLine( renderer, startX, startY, endX, endY );
}

void SDLWindow::rect( int x, int y, int w, int h, SDL_Color color ) {
	SDL_Rect rect = { x, y, w, h };
	setColor( color );
	SDL_RenderDrawRect( renderer, &rect );
}

void SDLWindow::fillRect( int x, int y, int w, int h, SDL_Color color ) {
	SDL_Rect rect = { x, y, w, h };
	setColor( color );
	SDL_RenderFillRect( renderer, &rect );
}

void SDLWindow::drawTexture( SDL_Rect rect, SDL_Texture* tex ) {
	SDL_RenderCopy( renderer, tex, NULL, &rect );
}

SDL_Surface* SDLWindow::loadSurface( string filename ) {
	SDL_Surface* surface = IMG_Load( filename.c_str() );
  if( !surface ) {
    error( "Couldn't load surface `%s`!", filename.c_str() );
  }
  return surface;
}

SDL_Texture* SDLWindow::loadTexture( string filename ) {
	auto iter = texes.find( filename );
	if( iter != texes.end() )	{
		warning( "%s already loaded", filename.c_str() );
		return iter->second;
	}
  
  SDL_Surface *surface = loadSurface( filename );
  if( !surface ) {
    error( "loadTexture couldn't load surface" );
    return 0;
  }
  
	SDL_Texture *tex = SDL_CreateTextureFromSurface( renderer, surface );
  if( !tex ) {
    error( "`%s`: SDL_CreateTextureFromSurface failed %s", filename.c_str(), SDL_GetError() );
    return 0;
  }
  
  // default textures to allow alpha blend.
	SDL_SetTextureBlendMode( tex, SDL_BLENDMODE_BLEND );
	texes[ filename ] = tex; // You can look up textures later also.
	return tex;
}

SDL_Texture* SDLWindow::makeText( TTF_Font* font, string text, SDL_Color color ) {
	SDL_Surface *messageSurface = TTF_RenderText_Solid( font, text.c_str(), color );
  if( !messageSurface ) {
    error( "TTF_RenderText_Solid failed %s", text.c_str() );
    return 0;
  }
  
	SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, messageSurface );
  
  if( !texture ) {
    error( "SDL_CreateTextureFromSurface failed %s", text.c_str() );
    return 0;
  }
	
  SDL_FreeSurface( messageSurface );
	return texture;
}

void SDLWindow::loadMusic( Music musicId, string filename ) {
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

void SDLWindow::loadWavSound( SFX sfxId, string waveFilename ) {
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

