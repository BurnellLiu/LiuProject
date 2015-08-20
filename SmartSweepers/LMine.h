
#ifndef _LMINE_H_
#define _LMINE_H_

#include "LWindowsGDI/LShape.h"
#include "LWindowsGDI/LWindowGDI.h"

/// @brief ���׼�����
class LMineSet : public IDrawTo
{
public:
	LMineSet();
	~LMineSet();

	/// @brief ���õ�������
	/// @param[in] num ��������
	void SetNum(IN int num);

	/// @brief ���õ�������Χ
	/// @param[in] width ���
	/// @param[in] height �߶�
	void SetArea(IN int width, IN int height);

	/// @brief ��ʼ�����׼���
	void Init();

	/// @brief ��ȡ����λ���б�
	/// @return ����λ���б�
	LPoint* GetPosList();

	/// @brief ����
	/// @param[in] hdc ��ָ����hdc�л���ͼ��
	virtual void DrawTo(IN HDC hdc);

private:
	int m_areaWidth; ///< ����������
	int m_areaHeight; ///< ��������߶�

	int m_mineNum; ///< ��������
	LShape m_mineShape; ///< ������״
	LPoint m_mineVBLocal[4]; ///< �������׶��㻺��(��������ϵ)
	LPoint m_mineVBWorld[4]; ///< �������׶��㻺��(��������ϵ)
	LPoint* m_minePosList; ///< ����λ���б�
};

#endif
