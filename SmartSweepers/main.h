
#ifndef _MAIN_H_
#define _MAIN_H_

#include "LSweeper.h"
#include "LMine.h"

#include "LOptimization.h"
#include "LWindowGDI.h"

#include<vector>
using std::vector;


class LGameWindow : public LWindow
{
public:
	LGameWindow();
	virtual ~LGameWindow();

	virtual void Exe();

private:
	void InitGame();
	void PaintGame();
	void RunGame();

	void PaintLineChart(IN HDC hdc);

	/// @brief 清理资源
	void CleanUp();

	/// @brief 按键弹起消息
	/// @param[in] wParam
	/// @param[in] lParam
	void KeyUpMessage(IN WPARAM wParam, IN LPARAM lParam);

	virtual LRESULT MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

private:
	HPEN	 m_hRedPen; ///< 红色画笔
	HPEN	 m_hBluePen; ///< 蓝色画笔
	HPEN	 m_hGreenPen; ///< 绿色画笔
	HPEN	 m_hOldPen; ///< 原始画笔

	LBackBufferDC m_backDC; ///< 背景缓冲区

	int m_mineNum; ///< 地雷数量
	LMineSet m_mineSet; ///< 地雷集合

	int m_weightListLen; ///< 扫雷机神经网络权重列表长度
	LNNWeightList m_weightList; ///< 扫雷机神经网络权重列表
	LSweeper* m_sweeperList; ///< 扫雷机列表
	int m_sweeperNum; ///< 扫雷机数量

	LOpenedGenetic m_geneticOptimize; ///< 遗传算法优化

	int m_generationCount; ///< 记录经历的遗传的代数
	int m_frameCount; ///< 记录刷新的帧数

	bool m_bFastGenerate; ///< 是否进行快速遗传

	vector<int> m_bestSocreList; ///< 每代最佳得分列表
	vector<int> m_avgSocoreList; ///< 每代平均得分列表
};

#endif