#pragma once

#include <graphics.h>

// ³¡¾°»ùÀà
class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() { }
	virtual void on_update() { }
	virtual void on_draw() { }
	virtual void on_input(const ExMessage& msg) { }
	virtual void on_exit() { }

private:

};