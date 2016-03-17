
#include "LShape.h"

#include <math.h>

void LShape::WorldTransform(IN LVertexBuffer& vbLocal, OUT LVertexBuffer& vbWorld) const
{
	float cosValue = cos(Rotation);
	float sinValue = sin(Rotation);

	if (vbWorld.Length != vbLocal.Length)
		vbWorld.Reset(vbLocal.Length);

	for (int i = 0; i < vbLocal.Length; i++)
	{
		LPoint& pointWorld = vbWorld.Data[i];
		const LPoint& pointLocal = vbLocal.Data[i];

		// 旋转
		pointWorld.X = pointLocal.X * cosValue - pointLocal.Y * sinValue;
		pointWorld.Y = pointLocal.X * sinValue + pointLocal.Y * cosValue;

		// 缩放
		pointWorld.X *= Scale;
		pointWorld.Y *= Scale;

		// 平移
		pointWorld.X += PosX;
		pointWorld.Y += PosY;
	}
}

void LShape::WorldTransform(IN LPoint* vbLocal, unsigned int size,OUT LPoint* vbWorld) const
{
    float cosValue = cos(Rotation);
    float sinValue = sin(Rotation);

   

    for (unsigned int i = 0; i < size; i++)
    {
        LPoint& pointWorld = vbWorld[i];
        const LPoint& pointLocal = vbLocal[i];

        // 旋转
        pointWorld.X = pointLocal.X * cosValue - pointLocal.Y * sinValue;
        pointWorld.Y = pointLocal.X * sinValue + pointLocal.Y * cosValue;

        // 缩放
        pointWorld.X *= Scale;
        pointWorld.Y *= Scale;

        // 平移
        pointWorld.X += PosX;
        pointWorld.Y += PosY;
    }
}