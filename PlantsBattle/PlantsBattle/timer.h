#pragma once

#include <functional>

/*
	计时器类有两种不同的实现思路：
	《通用定时器和摄像机特效》节
*/

// 计时器类
class Timer
{
public:
	Timer() = default;
	~Timer() = default;

private:
	int pass_time = 0; // 已过时间
	int wait_time = 0; // 等待时间
	bool paused = false; // 是否暂停 
	bool shotted = false; // 是否触发
	bool one_shot = false; // 单次触发
	std::function<void()> callback;
};

