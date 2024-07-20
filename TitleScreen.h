#pragma once

#include "Sprite.h"

class TitleScreen : public Sprite {
public:
	TitleScreen( const string &titleText );
	void update() override;
};
