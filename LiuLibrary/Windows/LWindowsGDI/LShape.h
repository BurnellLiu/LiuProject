
#ifndef _LSHAPE_H_
#define _LSHAPE_H_

#include "LMacroDefine.h"
#include "LDataStruct/include/LArray.h"

#ifndef PI 
#define	PI 3.14159265358979f
#endif


#ifndef MAX_ROTATION_PER_TICK
#define MAX_ROTATION_PER_TICK 0.05f ///< ÿ֡ʱ�������ת�Ƕ�
#endif

#ifndef MAX_SCALE_PER_TICK
#define MAX_SCALE_PER_TICK 0.1f ///< ÿ֡ʱ��������ű���
#endif

#ifndef MAX_TRANSLATION_PER_TICK
#define MAX_TRANSLATION_PER_TICK 1 ///< ÿ֡ʱ������ƶ�����(����)
#endif

/// @brief ������ṹ
struct LPoint
{
	LPoint(float x = 0.0f, float y = 0.0f) : X(x), Y(y){}

	float X; ///< X������
	float Y; ///< Y������
};

typedef LArray<LPoint> LVertexBuffer; ///< ���㻺����

/// @brief ��״�ṹ
struct LShape
{
	LShape()
	{
		PosX = 0.0f;
		PosY = 0.0f;
		Rotation = 0.0f;
		Scale = 1.0f;
	}

	/// @brief ����任(�����㻺��ӱ�������ϵת������������ϵ)
	/// @param[in] vbLocal ���ض��㻺��
	/// @param[out] vbWorld ���綥�㻺��
	void WorldTransform(IN LVertexBuffer& vbLocal, OUT LVertexBuffer& vbWorld) const;

	float PosX; ///< ��״���ĵ�x����(��������ϵ)
	float PosY; ///< ��״���ĵ�y����(��������ϵ)

	float Rotation; ///< ��״�����y���ƫת�Ƕ�(����)(���Ӹ�ֵ˳ʱ����ת, ��С��ֵ��ʱ����ת)

	float Scale; ///< ��״��Ҫ�ķŴ����
};

#endif