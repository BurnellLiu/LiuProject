#ifndef _LCOLLISION_H_
#define _LCOLLISION_H_

#include "LMacroDefine.h"
#include "LWindowsGDI/LShape.h"

/// @brief �ж�2D�ռ��е��߶�AB��CD�Ƿ��ཻ
/// @param[in] A
/// @param[in] B
/// @param[in] C
/// @param[in] D
/// @param[out] dist ���AB��CD�ཻ��ôdist�洢A���ཻ�������AB���ȵı�ֵ(0~1),  ������ཻdist(-1)
/// @return �ཻ����true, ���ཻ����false
bool LineIntersection2D(
	IN const LPoint& A,
	IN const LPoint& B,
	IN const LPoint& C, 
	IN const LPoint& D,
	OUT float& dist);
#endif