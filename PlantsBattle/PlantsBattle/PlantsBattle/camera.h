#pragma once

#include "vector2.h"
#include "timer.h"

// 摄像机类
class Camera
{
public:
	Camera()
	{
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			});
	}

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
		timer_shake.on_update(delta);

		if (is_shaking) // 在单位圆内随机取坐标完成抖动
		{
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}

	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}

private:
	Vector2 position; // 摄像机位置
	Timer timer_shake; // 摄像机抖动定时器
	bool is_shaking = false; // 摄像机是否正在抖动
	float shaking_strength = 0; // 摄像机抖动幅度
};

