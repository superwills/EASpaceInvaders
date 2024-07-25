#pragma once

struct Range {
  int min, max;
};
struct RangeF {
  float min, max;
};

struct ParticleCloudProperties {
  Range number;
  RangeF size;
  
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
