
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

		// Ðý×ª
		pointWorld.X = pointLocal.X * cosValue - pointLocal.Y * sinValue;
		pointWorld.Y = pointLocal.X * sinValue + pointLocal.Y * cosValue;

		// Ëõ·Å
		pointWorld.X *= Scale;
		pointWorld.Y *= Scale;

		// Æ½ÒÆ
		pointWorld.X += PosX;
		pointWorld.Y += PosY;
	}
}