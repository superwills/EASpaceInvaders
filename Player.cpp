#include "Player.h"

#include "Bullet.h"
#include "SDLWindow.h"

Player::Player( const RectF &rectangle ) : Sprite( rectangle, AnimationId::Player ) {
  name = "Player/" + name;
}

void Player::die() {
  Sprite::die();
  //if( dead ) {
  //  error( "Player double die call" );
  //}
  // swap out the player death sprite.
  animation = sdl->getAnimation( AnimationId::PlayerDie );
}
