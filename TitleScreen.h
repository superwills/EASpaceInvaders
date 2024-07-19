#pragma once

#include "Sprite.h"

class TitleScreen : public Sprite {
public:
	TitleScreen(string title, string filename);
	void update();
};
