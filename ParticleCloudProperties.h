#pragma once

struct ParticleCloudProperties {
  int minParticles = 8, maxParticles = 12;
  float particleSizeMin = 4, particleSizeMax = 12;
  
  // You can start the particle decayed, so that it doesn't start at 100% alpha with initialDecay
  float initialParticleDecay = .9;
  
  inline bool isOn() const {
     return maxParticles;
  }
  
  inline void setNumParticles( int min, int max ) {
    minParticles = min;
    maxParticles = max;
  }
  
  inline void setSizes( float min, float max ) {
    particleSizeMin = min;
    particleSizeMax = max;
  }
};
