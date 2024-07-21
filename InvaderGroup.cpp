#include "InvaderGroup.h"

#include "Invader.h"

void InvaderGroup::move() {
  
}

void InvaderGroup::draw() const {
  for( shared_ptr<Invader> invader : invaders ) {
    invader->draw();
  }
}

void InvaderGroup::clearDead() {
  invaders.erase( std::remove_if( invaders.begin(), invaders.end(), []( shared_ptr<Invader> invader ) {
    return invader->dead;
  } ), invaders.end() );
}
