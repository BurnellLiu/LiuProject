
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

	/// @brief ������Դ
	void CleanUp();

	/// @brief ����������Ϣ
	/// @param[in] wParam
	/// @param[in] lParam
	void KeyUpMessage(IN WPARAM wParam, IN LPARAM lParam);

	virtual LRESULT MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

private:
	HPEN	 m_hRedPen; ///< ��ɫ����
	HPEN	 m_hBluePen; ///< ��ɫ����
	HPEN	 m_hGreenPen; ///< ��ɫ����
	HPEN	 m_hOldPen; ///< ԭʼ����

	LBackBufferDC m_backDC; ///< ����������

	int m_mineNum; ///< ��������
	LMineSet m_mineSet; ///< ���׼���

	int m_weightListLen; ///< ɨ�׻�������Ȩ���б���
	LNNWeightList m_weightList; ///< ɨ�׻�������Ȩ���б�
	LSweeper* m_sweeperList; ///< ɨ�׻��б�
	int m_sweeperNum; ///< ɨ�׻�����

	LOpenedGenetic m_geneticOptimize; ///< �Ŵ��㷨�Ż�

	int m_generationCount; ///< ��¼�������Ŵ��Ĵ���
	int m_frameCount; ///< ��¼ˢ�µ�֡��

	bool m_bFastGenerate; ///< �Ƿ���п����Ŵ�

	vector<int> m_bestSocreList; ///< ÿ����ѵ÷��б�
	vector<int> m_avgSocoreList; ///< ÿ��ƽ���÷��б�
};

#endif