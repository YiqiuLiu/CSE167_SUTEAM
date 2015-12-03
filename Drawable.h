#pragma once
#include "Shader.h"
class Drawable
{
public:
	static enum object_state{
		ALIVE,
		DYING,
		DEAD
	};

	object_state state;
	Drawable();
	~Drawable();
	virtual void draw(Shader);
	virtual void update();
};

