#pragma once

struct Range {
  int min = 0, max = 0;
  
  Range() { }
  Range( int iMin, int iMax ) :
    min( iMin ), max( iMax ) { }
};
struct RangeF {
  float min = 0, max = 0;
  
  RangeF() { }
  RangeF( float iMin, float iMax ) :
    min( iMin ), max( iMax ) { }
};

struct ParticleCloudProperties {
  Range number = Range( 4, 8 );
  RangeF size = RangeF( 8, 12 );
  
  // You can start the particle decayed, so that it doesn't start at 100% alpha with initialDecay
  float initialDecay = .9;
  
  inline bool isOn() const {
     return number.max >= 0;
  }
  
  inline void setNumParticles( int min, int max ) {
    number.min = min;
    number.max = max;
  }
  
  inline void setSizes( float min, float max ) {
    size.min = min;
    size.max = max;
  }
};
