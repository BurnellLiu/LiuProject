

#ifndef _LSWEEPER_H_
#define _LSWEEPER_H_

#include "LShape.h"
#include "LWindowGDI.h"
#include "LVector2D.h"

#include "LNeuralNetwork.h"

/// @brief 扫雷机类
class LSweeper : IPaint
{
public:
	LSweeper();
	~LSweeper();

	/// @brief 设置扫雷机活动区域
	/// @param[in] width 宽度
	/// @param[in] height 高度
	/// @return 
	void SetLiveArea(IN float width, IN float height);

	/// @brief 初始化扫雷机
	void Init();

	/// @brief 更新扫雷机状态
	/// @param[in] minePosList 地雷位置列表
	void Update(IN LPoint* minePosList, IN int mineNum);

	/// @brief 绘制扫雷机
	/// @param[in] hdc 在指定的hdc中绘制图形
	virtual void Paint(IN HDC hdc);

	/// @brief 获取扫雷机的得分
	/// @return 分数
	int GetScore();

	/// @brief 重置扫雷机得分
	void ResetScore();

	/// @brief 设置扫雷机神经网络权重
	/// @param[in] weightList 权重列表
	void SetNNWeight(IN LNNWeightList& weightList);

	/// @brief 获取切割点列表
	/// @param splitPointList 切割点列表 
	void GetNNSplitPointList(INOUT LNNSplitPointList& splitPointList);

	/// @brief 获取权重数量
	/// @return 权重数量
	int GetNNWeightNum();
	
private:
	static LPoint s_sweeperVBLocal[16]; ///< 扫雷机顶点缓存(本地坐标系)
	static LPoint s_sweeperVBWorld[16]; ///< 扫雷机顶点缓存(本地坐标系)

	LShape m_sweeperShape; ///< 扫雷机形状
	LVector2D m_vLookAt; ///< 扫雷机视线向量
	float m_liveWidth; ///< 扫雷机活动区域宽度
	float m_liveHeight; ///< 扫雷机活动区域高度

	LNNInputList m_inputParamList; ///< 扫雷机神经网络输入参数列表
	LNNOutputList m_outputParamList; ///< 扫雷机神经网络输出参数列表
	LUnSuperviseNN m_neuronNet; ///< 扫雷机的神经网络
	int m_score; ///< 扫雷机得分
};

#endif