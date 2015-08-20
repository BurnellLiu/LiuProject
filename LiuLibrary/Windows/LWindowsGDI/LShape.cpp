
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

		// ��ת
		pointWorld.X = pointLocal.X * cosValue - pointLocal.Y * sinValue;
		pointWorld.Y = pointLocal.X * sinValue + pointLocal.Y * cosValue;

		// ����
		pointWorld.X *= Scale;
		pointWorld.Y *= Scale;

		// ƽ��
		pointWorld.X += PosX;
		pointWorld.Y += PosY;
	}
}