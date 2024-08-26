#include <graphics.h>
#include <string>

int idx_current_anim = 0; // 存储当前动画帧索引
const int PLAYER_ANIM_NUM = 6; // 动画帧总数常量
const int PLAYER_SPEED = 3; // 玩家速度

// 动画存储数组
IMAGE img_player_left[PLAYER_ANIM_NUM];
IMAGE img_player_right[PLAYER_ANIM_NUM];

POINT player_pos = { 500, 500 }; // 存储玩家位置

// 支持透明度的渲染函数
#pragma comment(lib, "MSIMG32.LIB")
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();

	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

}

// 加载动画
void LoadAnimation()
{
	for (size_t i = 0; i < PLAYER_ANIM_NUM; i++)
	{
		std::wstring path = L"img/player_left_" + std::to_wstring(i) + L".png";
		loadimage(&img_player_left[i], path.c_str());
	}

	for (size_t i = 0; i < PLAYER_ANIM_NUM; i++)
	{
		std::wstring path = L"img/player_right_" + std::to_wstring(i) + L".png";
		loadimage(&img_player_right[i], path.c_str());
	}
}

int main()
{
	initgraph(1280, 720); // 初始化窗口为1280*720

	bool running = true; // 控制游戏是否进行

	ExMessage msg;
	IMAGE img_background; // 背景图

	bool is_move_up = false;
	bool is_move_down = false;
	bool is_move_left = false;
	bool is_move_right = false;

	loadimage(&img_background, _T("img/background.png")); // 加载背景图
	LoadAnimation();

	BeginBatchDraw();

	while(running)
	{
		DWORD start_time = GetTickCount();

		// 输入处理
		while (peekmessage(&msg))
		{
			if (msg.message == WM_KEYDOWN)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
					is_move_up = true;
					break;
				case VK_DOWN:
					is_move_down = true;
					break;
				case VK_LEFT:
					is_move_left = true;
					break;
				case VK_RIGHT:
					is_move_right = true;
					break;
				}
			}
			else if (msg.message == WM_KEYUP)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
					is_move_up = false;
					break;
				case VK_DOWN:
					is_move_down = false;
					break;
				case VK_LEFT:
					is_move_left = false;
					break;
				case VK_RIGHT:
					is_move_right = false;
					break;
				}
			}
		}

		// 数据处理
		if (is_move_up)
			player_pos.y -= PLAYER_SPEED;
		if (is_move_down)
			player_pos.y += PLAYER_SPEED;
		if (is_move_left)
			player_pos.x -= PLAYER_SPEED;
		if (is_move_right)
			player_pos.x += PLAYER_SPEED;

		static int counter = 0; // 记录当前动画帧共播放几个游戏帧
		if (++counter % 5 == 0) // 每5帧切换一次动画
		{
			idx_current_anim++;
		}

		idx_current_anim %= PLAYER_ANIM_NUM; // 使动画循环

		// 渲染画面
		cleardevice();

		putimage(0, 0, &img_background);
		putimage_alpha(player_pos.x, player_pos.y, &img_player_left[idx_current_anim]);

		FlushBatchDraw();

		// 稳定刷新率
		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / 144)
		{
			Sleep(1000 / 144 - delta_time);
		}
	}
	
	EndBatchDraw();

	return 0;
}