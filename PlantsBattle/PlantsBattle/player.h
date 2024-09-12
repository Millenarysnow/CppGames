#pragma once

#include "camera.h"
#include "vector2.h"
#include "animation.h"
#include "player_id.h"
#include "platform.h"
#include <graphics.h>

extern std::vector<Platform> platform_list;
class Player
{
public:
	Player()
	{
		current_animation = &animation_idle_right;
	}

	~Player() = default;

	virtual void on_update(int delta)
	{
		int direction = is_right_key_down - is_left_key_down;
		
		if (direction != 0)
		{
			is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else
		{
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
		}

		current_animation->on_update(delta);

		move_and_collide(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		current_animation->on_draw(camera, (int)position.x, (int)position.y);
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41: // A
					is_left_key_down = true;
					break;
				case 0x44: // D
					is_right_key_down = true;
					break;
				case 0x57: // W
					on_jump();
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT: // <-
					is_left_key_down = true;
					break;
				case	VK_RIGHT: // ->
					is_right_key_down = true;
					break;
				case VK_UP: // ^
					on_jump();
					break;
				}
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
				case 0x41: // A
					is_left_key_down = false;
					break;
				case 0x44: // D
					is_right_key_down = false;
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT: // <-
					is_left_key_down = false;
					break;
				case	VK_RIGHT: // ->
					is_right_key_down = false;
					break;
				}
				break;
			}
			break;
		}
	}

	virtual void on_run(float distance)
	{
		position.x += distance;
	}

	virtual void on_jump()
	{
		if (velocity.y != 0)
			return;

		velocity.y += jump_velocity;
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}

	void set_position(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

protected:
	// 物理相关
	void move_and_collide(int delta)
	{
		velocity.y += gravity * delta;
		position += velocity * (float)delta;

		// 平台单向碰撞检测
		if (velocity.y > 0)
		{
			for (const Platform& platform : platform_list)
			{
				const Platform::CollisionShape& shape = platform.shape;
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left)
					<= size.x + (shape.right - shape.left));
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y)
				{
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					
					if (last_tick_foot_pos_y <= shape.y)
					{
						position.y = shape.y - size.y;
						velocity.y = 0;

						break;
					}
				}
			}
		}
	}

protected:
	const float gravity = 1.6e-3f; // 玩家重力
	const float run_velocity = 0.55f; // 跑动速度
	const float jump_velocity = -0.85f; // 跳跃速度

protected:
	Vector2 size; // 角色尺寸
	Vector2 position; // 角色位置
	Vector2 velocity; // 角色速度

	Animation animation_idle_left; // 朝左的默认动画
	Animation animation_idle_right; // 朝右的默认动画
	Animation animation_run_left; // 朝左的奔跑动画
	Animation animation_run_right; // 朝右的奔跑动画

	Animation* current_animation = nullptr; // 当前正在播放的动画

	PlayerID id = PlayerID::P1; // 玩家序号ID

	bool is_left_key_down = false; // 向左是否按下
	bool is_right_key_down = false; // 向右是否按下

	bool is_facing_right = true; // 角色是否向右
};