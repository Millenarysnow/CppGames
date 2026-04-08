#pragma once

#include <vector>
#include <graphics.h>

// 图集类
class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	// 加载图片
	void load_from_file(LPCTSTR path_template, int num)
	{
		img_list.clear();
		img_list.resize(num);

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + 1);
			loadimage(&img_list[i], path_file);
		}
	}

	// 清空图集
	void clear()
	{
		img_list.clear();
	}

	// 获取图集中图片数量
	int get_size()
	{
		return (int)img_list.size();
	}

	// 获取实际渲染的动画帧
	IMAGE* get_image(int idx)
	{
		if (idx < 0 || idx >= img_list.size())
			return nullptr;

		return &img_list[idx];
	}

	// 添加已有的图片
	void add_image(const IMAGE& img)
	{
		img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;
};
