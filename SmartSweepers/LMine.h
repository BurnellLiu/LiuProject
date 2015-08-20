
#ifndef _LMINE_H_
#define _LMINE_H_

#include "LWindowsGDI/LShape.h"
#include "LWindowsGDI/LWindowGDI.h"

/// @brief 地雷集合类
class LMineSet : public IDrawTo
{
public:
	LMineSet();
	~LMineSet();

	/// @brief 设置地雷数量
	/// @param[in] num 地雷数量
	void SetNum(IN int num);

	/// @brief 设置地雷区域范围
	/// @param[in] width 宽度
	/// @param[in] height 高度
	void SetArea(IN int width, IN int height);

	/// @brief 初始化地雷集合
	void Init();

	/// @brief 获取地雷位置列表
	/// @return 地雷位置列表
	LPoint* GetPosList();

	/// @brief 绘制
	/// @param[in] hdc 在指定的hdc中绘制图形
	virtual void DrawTo(IN HDC hdc);

private:
	int m_areaWidth; ///< 地雷区域宽度
	int m_areaHeight; ///< 地雷区域高度

	int m_mineNum; ///< 地雷数量
	LShape m_mineShape; ///< 地雷形状
	LPoint m_mineVBLocal[4]; ///< 单个地雷顶点缓存(本地坐标系)
	LPoint m_mineVBWorld[4]; ///< 单个地雷顶点缓存(世界坐标系)
	LPoint* m_minePosList; ///< 地雷位置列表
};

#endif
