#include "Ball.h"
#include "Game.h"

Ball::Ball( float size ) {
	lastStartSpeed = 2;
	box.w = box.h = size;
}

void Ball::saveLastStartSpeed() {
	
}

void Ball::update() {
	
	// don't exit top or bottom
	enforceWorldLimits();
	
	
}
