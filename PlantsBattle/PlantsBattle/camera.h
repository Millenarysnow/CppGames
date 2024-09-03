#pragma once

#include "vector2.h"

// 摄像机类
class Camera
{
public:
	Camera() = default;
	~Camera() = default;

	const Vector2& get_position() const
	{
		return position;
	} // 第一个const修饰返回值，设定返回值为常量引用

	void reset()
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		const Vector2 speed = { -0.35f, 0 };
		position += speed * (float)delta;
	}

private:
	Vector2 position; // 位置
};

