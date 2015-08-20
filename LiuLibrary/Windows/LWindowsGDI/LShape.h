
#ifndef _LSHAPE_H_
#define _LSHAPE_H_

#include "LMacroDefine.h"
#include "LDataStruct/include/LArray.h"

#ifndef PI 
#define	PI 3.14159265358979f
#endif


#ifndef MAX_ROTATION_PER_TICK
#define MAX_ROTATION_PER_TICK 0.05f ///< 每帧时间最大旋转角度
#endif

#ifndef MAX_SCALE_PER_TICK
#define MAX_SCALE_PER_TICK 0.1f ///< 每帧时间最大缩放比例
#endif

#ifndef MAX_TRANSLATION_PER_TICK
#define MAX_TRANSLATION_PER_TICK 1 ///< 每帧时间最大移动距离(像素)
#endif

/// @brief 点坐标结构
struct LPoint
{
	LPoint(float x = 0.0f, float y = 0.0f) : X(x), Y(y){}

	float X; ///< X轴坐标
	float Y; ///< Y轴坐标
};

typedef LArray<LPoint> LVertexBuffer; ///< 顶点缓冲区

/// @brief 形状结构
struct LShape
{
	LShape()
	{
		PosX = 0.0f;
		PosY = 0.0f;
		Rotation = 0.0f;
		Scale = 1.0f;
	}

	/// @brief 世界变换(将顶点缓存从本地坐标系转换到世界坐标系)
	/// @param[in] vbLocal 本地顶点缓存
	/// @param[out] vbWorld 世界顶点缓存
	void WorldTransform(IN LVertexBuffer& vbLocal, OUT LVertexBuffer& vbWorld) const;

	float PosX; ///< 形状中心的x坐标(世界坐标系)
	float PosY; ///< 形状中心的y坐标(世界坐标系)

	float Rotation; ///< 形状相对与y轴的偏转角度(弧度)(增加该值顺时针旋转, 减小该值逆时针旋转)

	float Scale; ///< 形状需要的放大比例
};

#endif