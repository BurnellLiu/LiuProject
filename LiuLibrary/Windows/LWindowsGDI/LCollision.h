#ifndef _LCOLLISION_H_
#define _LCOLLISION_H_

#include "LMacroDefine.h"
#include "LWindowsGDI/LShape.h"

/// @brief 判断2D空间中的线段AB和CD是否相交
/// @param[in] A
/// @param[in] B
/// @param[in] C
/// @param[in] D
/// @param[out] dist 如果AB和CD相交那么dist存储A到相交点距离与AB长度的比值(0~1),  如果不相交dist(-1)
/// @return 相交返回true, 不相交返回false
bool LineIntersection2D(
	IN const LPoint& A,
	IN const LPoint& B,
	IN const LPoint& C, 
	IN const LPoint& D,
	OUT float& dist);
#endif