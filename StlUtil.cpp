#include "StlUtil.h"

string makeString( const char *fmt, ... ) {
  va_list args ;
  va_start( args, fmt ) ;
  const int Size = 4096;
  char msgBuffer[ Size ] ; // max len.  Can also use a strlen call to avoid over alloc
  vsnprintf( msgBuffer, Size, fmt, args ) ;
  return string( msgBuffer ) ; // ALWAYS COPIES WHAT'S IN MSGBUFFER.
}

string makeTimeString( double s ) {
  uint64_t floorSec = s ;
  uint64_t hours = floorSec / (60 * 60) ;
  int minutes = (floorSec / 60) % 60 ;
  int seconds = floorSec % 60 ;

  double ms = s - floorSec ;
  int msInt = (int)(ms*1000.) ;
  
  if( !hours )
    return makeString( "%d:%02d.%03d", minutes, seconds, msInt ) ;
  else
    return makeString( "%lld:%02d:%02d.%03d", hours, minutes, seconds, msInt ) ;
}
