#include "Player.h"

#include "Bullet.h"
#include "SDLWindow.h"

Player::Player( const RectF &rectangle ) : Sprite( rectangle, AnimationId::Player ) {
  name = "Player/" + name;
}

