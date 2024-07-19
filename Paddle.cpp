#include "Paddle.h"

Paddle::Paddle(Vector2f size, float iSpeed, string iname):Sprite(iname) {
  box.w = size.x;
  box.h = size.y;
	speed = iSpeed;
}

void Paddle::moveDown()
{
  box.y += speed;
	bounceTopAndBottom();
}

void Paddle::moveUp()
{
  box.y -= speed;
	bounceTopAndBottom();
}
