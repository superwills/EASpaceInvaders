#include "Log.h"

// LOGGING
const char* progname = "SI" ;

//  Info    = 1 << 0, // 1
//  Warning = 1 << 1, // 2
//  Error   = 1 << 2  // 4
const char* ErrorLevelName[] = {
	"", //0
	"Info", //1
	"Warning", //2
	"",//3
	"Error", //4
	"","","" //5,6,7
} ;

tm* getCurrentTime() {
	static time_t raw ;

	// grab the current time
	time( &raw ) ;

	// Now create that timeinfo struct
	static tm* timeinfo ;
	timeinfo = localtime( &raw ) ;

	return timeinfo ;
}

char* getCurrentTimeString()
{
	// write time into timeBuff
	static char timeBuf[ 256 ] ;
	strftime( timeBuf, 255, "%c", getCurrentTime() ) ;
	return timeBuf ;
}

// decorates the log message with [appname][thread][error level][current time]:  message
void logDecorate( int logLevel, short color, const char *fmt, va_list args )
{
	const int Size = 4096;
	char msgBuffer[ Size ] ;  // oops. Had a 623 char error (from shader) and it err-d out.
	vsnprintf( msgBuffer, Size, fmt, args ) ;

	// write time into timeBuff. Should be about 8 chars hh:mm:ss
	char timeBuf[ 32 ] ;
	strftime( timeBuf, 255, "%X", getCurrentTime() ) ;

	// Put it all together
	char buf[ Size ] ;

	snprintf( buf, Size, "[ %s ][ %s ][ %s ]:  %s", progname, ErrorLevelName[ logLevel ], timeBuf, msgBuffer ) ;

	//printf( "%s\n", buf ) ; // don't want inserted.
	puts( buf ) ;
}

void error( const char *fmt, ... )
{
	va_list lp ;
	va_start( lp, fmt ) ;

	logDecorate( ErrorLevel::Error, ConsoleRed, fmt, lp ) ;
}

void warning( const char *fmt, ... )
{
	va_list lp ;
	va_start( lp, fmt ) ;

	logDecorate( ErrorLevel::Warning, ConsoleYellow, fmt, lp ) ;
}

void info( short iColor, const char *fmt, ... )
{
	va_list lp ;
	va_start( lp, fmt ) ;

	logDecorate( ErrorLevel::Info, iColor, fmt, lp ) ;
}

void info( const char *fmt, ... )
{
	va_list lp ;
	va_start( lp, fmt ) ;

	logDecorate( ErrorLevel::Info, ConsoleGray, fmt, lp ) ;
}
