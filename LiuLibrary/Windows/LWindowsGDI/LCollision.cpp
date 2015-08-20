#include "LWindowsGDI/LCollision.h"

bool LineIntersection2D(
	IN const LPoint& A,
	IN const LPoint& B,
	IN const LPoint& C, 
	IN const LPoint& D,
	OUT float&dist)
{

	float rTop = (A.Y-C.Y)*(D.X-C.X)-(A.X-C.X)*(D.Y-C.Y);
	float rBot = (B.X-A.X)*(D.Y-C.Y)-(B.Y-A.Y)*(D.X-C.X);

	float sTop = (A.Y-C.Y)*(B.X-A.X)-(A.X-C.X)*(B.Y-A.Y);
	float sBot = (B.X-A.X)*(D.Y-C.Y)-(B.Y-A.Y)*(D.X-C.X);

	if ((rBot == 0) || (sBot == 0))
	{
		//Æ½ÐÐ
		dist = -1;
		return false;
	}

	float r = rTop/rBot;
	float s = sTop/sBot;

	if((r > 0) && (r < 1.0f) && (s > 0) && (s < 1.0f))
	{
		dist = r;
		return true;
	}
	else
	{
		dist = -1;
		return false;
	}
}