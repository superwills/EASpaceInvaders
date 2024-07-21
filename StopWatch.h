#pragma once

#include <chrono>
#include <string>
using std::string;

struct StopWatch {
  string name;
  bool autoprint;
  std::chrono::high_resolution_clock::time_point start;
  
  StopWatch( string iname = "unnamed timer", bool iAutoprint = 0 );
  ~StopWatch();
  void reset();
  unsigned long long milli() const;
  double sec() const;
  void spin( double fps );
};

struct Every {
  StopWatch watch;
  double interval = 0, last = 0;

  Every( double iInterval ) : interval( iInterval ) {
  }

  bool isTime() {
    double now = watch.sec();
    double diff = now - last;
    bool istime = 0;
    if( diff > interval ) {
      istime = true;
      last = now;
    }
    return istime;
  }
};

struct After {
  StopWatch watch;
  double when = 1.;
  bool done = 0;

  After( double iWhen ) : when( iWhen ) {
  }

  bool isTime() {
    // Only returns true ONCE.
    if( done )  return 0;

    if( watch.sec() > when )
      done = 1;
    
    return done;
  }
};


