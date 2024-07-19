#include "Ball.h"
#include "Game.h"

Ball::Ball(float size, string iname):Sprite( iname )
{
	lastStartSpeed = 2;
	box.w = box.h = size;
	color.b = 0;
}

void Ball::saveLastStartSpeed() {
	lastStartSpeed = vel.len();
}

void Ball::update()
{
	vel *= 1.0001f;

	// If the x-update()ment speed is too slow, make it -1 or +1
	if( fabsf( vel.x ) < 1 )
		vel.x = signum(vel.x);
	box.xy() += vel;
	
	// don't exit top or bottom
	bounceTopAndBottom();
	
	if( box.left() < 0 )
	{
		game->rightPlayerScored();
	}
	if( box.right() > sdl->getSize().x )
	{
		game->leftPlayerScored();
	}
}
