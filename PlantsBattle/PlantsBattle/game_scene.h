#pragma once

#include "util.h"
#include "scene.h"
#include "scene_manager.h"

extern IMAGE img_sky; // 天空图片
extern IMAGE img_hills; // 山脉图片
extern IMAGE img_platform_large; // 大型平台图片
extern IMAGE img_platform_small; // 小型平台图片

extern Camera main_camera;

extern SceneManager scene_manager;

// 游戏场景
class GameScene :public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter()
	{
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
	}

	void on_update(int delta)
	{
		
	}

	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);
	}

	void on_input(const ExMessage& msg)
	{
		
	}

	void on_exit()
	{
		
	}

private:
	POINT pos_img_sky = { 0 }; // 天空背景图位置
	POINT pos_img_hills = { 0 }; // 山脉背景图位置
};
