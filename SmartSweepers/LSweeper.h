

#ifndef _LSWEEPER_H_
#define _LSWEEPER_H_

#include "LShape.h"
#include "LWindowGDI.h"
#include "LVector2D.h"

#include "LNeuralNetwork.h"

/// @brief ɨ�׻���
class LSweeper : IPaint
{
public:
	LSweeper();
	~LSweeper();

	/// @brief ����ɨ�׻������
	/// @param[in] width ���
	/// @param[in] height �߶�
	/// @return 
	void SetLiveArea(IN float width, IN float height);

	/// @brief ��ʼ��ɨ�׻�
	void Init();

	/// @brief ����ɨ�׻�״̬
	/// @param[in] minePosList ����λ���б�
	void Update(IN LPoint* minePosList, IN int mineNum);

	/// @brief ����ɨ�׻�
	/// @param[in] hdc ��ָ����hdc�л���ͼ��
	virtual void Paint(IN HDC hdc);

	/// @brief ��ȡɨ�׻��ĵ÷�
	/// @return ����
	int GetScore();

	/// @brief ����ɨ�׻��÷�
	void ResetScore();

	/// @brief ����ɨ�׻�������Ȩ��
	/// @param[in] weightList Ȩ���б�
	void SetNNWeight(IN LNNWeightList& weightList);

	/// @brief ��ȡ�и���б�
	/// @param splitPointList �и���б� 
	void GetNNSplitPointList(INOUT LNNSplitPointList& splitPointList);

	/// @brief ��ȡȨ������
	/// @return Ȩ������
	int GetNNWeightNum();
	
private:
	static LPoint s_sweeperVBLocal[16]; ///< ɨ�׻����㻺��(��������ϵ)
	static LPoint s_sweeperVBWorld[16]; ///< ɨ�׻����㻺��(��������ϵ)

	LShape m_sweeperShape; ///< ɨ�׻���״
	LVector2D m_vLookAt; ///< ɨ�׻���������
	float m_liveWidth; ///< ɨ�׻��������
	float m_liveHeight; ///< ɨ�׻������߶�

	LNNInputList m_inputParamList; ///< ɨ�׻���������������б�
	LNNOutputList m_outputParamList; ///< ɨ�׻���������������б�
	LUnSuperviseNN m_neuronNet; ///< ɨ�׻���������
	int m_score; ///< ɨ�׻��÷�
};

#endif