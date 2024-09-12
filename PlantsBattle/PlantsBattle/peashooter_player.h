#pragma once

#include "player.h"
#include <iostream>

extern Atlas atlas_peashooter_idle_left; // 豌豆射手向左的默认动画图集
extern Atlas atlas_peashooter_idle_right; // 豌豆射手向右的默认动画图集
extern Atlas atlas_peashooter_run_left; // 豌豆射手向左的奔跑动画图集
extern Atlas atlas_peashooter_run_right; // 豌豆射手向右的奔跑动画图集

class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer()
	{
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);
	
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		size.x = 96;
		size.y = 96;
	}

	~PeashooterPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		std::cout << "豌豆射手正在更新……" << std::endl;
	}

private:

};
