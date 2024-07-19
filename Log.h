#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include <string>
using namespace std;

/////////////////LOGGING

enum ErrorLevel {
  Info    = 1 << 0, // 1
  Warning = 1 << 1, // 2
  Error   = 1 << 2  // 4
} ;

enum ConsoleColor
{
  ConsoleRed,ConsoleYellow,ConsoleGray
} ;

double getClockS() ;
extern const char* progname ;
extern const char* ErrorLevelName[] ;

tm* getCurrentTime();

char* getCurrentTimeString();

// decorates the log message with [appname][thread][error level][current time]:  message
void logDecorate( int logLevel, short color, const char *fmt, va_list args );

string logDecorateGetString( int logLevel, const char *fmt, va_list args ) ;

void error( const char *fmt, ... );

void warning( const char *fmt, ... );

void info( short iColor, const char *fmt, ... );

void info( const char *fmt, ... );

