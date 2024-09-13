#pragma once

#include "player.h"
#include "sun_bullet.h"
#include "sun_bullet_ex.h"
#include <iostream>

extern Atlas atlas_sunflower_idle_left; // 向日葵向左的默认动画图集
extern Atlas atlas_sunflower_idle_right; // 向日葵向右的默认动画图集
extern Atlas atlas_sunflower_run_left; // 向日葵向左的奔跑动画图集
extern Atlas atlas_sunflower_run_right; // 向日葵向右的奔跑动画图集
extern Atlas atlas_sunflower_attack_ex_left; // 向日葵向左的特殊攻击动画图集
extern Atlas atlas_sunflower_attack_ex_right; // 向日葵向右的特殊攻击动画图集
extern Atlas atlas_sun_text; // “日”文本动画图集

class SunflowerPlayer : public Player
{
public:
	SunflowerPlayer()
	{
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		animation_sun_text.set_atlas(&atlas_sun_text);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(100);
		animation_attack_ex_right.set_interval(100);
		animation_sun_text.set_interval(100);

		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_sun_text.set_loop(false);

		size.x = 96;
		size.y = 96;

		animation_attack_ex_left.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});
		animation_attack_ex_right.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});

		attack_cd = 250;
	}

	~SunflowerPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		if (is_sun_text_visible)
			animation_sun_text.on_update(delta);
	}

	void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);

		if (is_sun_text_visible)
		{
			Vector2 text_position;
			IMAGE* frame = animation_sun_text.get_frame();
			text_position.x = position.x - (size.x - frame->getwidth()) / 2;
			text_position.y = position.y - frame->getheight();
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);
		}
	}

	void on_attack()
	{

	}
	
private:
	const float speed_sun_ex = 0.15f; // 大型日光炸弹下落速度
	const Vector2 velocity_sun = { 0.25f, -0.25f }; // 小型日光炸弹抛射速度

private:
	Animation animation_sun_text; // 头顶文本动画
	bool is_sun_text_visible = false; // 是否显示头顶文本
};