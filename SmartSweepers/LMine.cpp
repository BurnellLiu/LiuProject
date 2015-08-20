
#include "LMine.h"

const LPoint g_mine[4] = 
{
	LPoint(-1, -1),
	LPoint(-1, 1),
	LPoint(1, 1),
	LPoint(1, -1)
};

LMineSet::LMineSet()
{
	m_areaWidth = 0;
	m_areaHeight = 0;
	m_mineNum = 0;
}

LMineSet::~LMineSet()
{
	SAFE_DELETE_ARR(m_minePosList);
}

void LMineSet::SetArea(IN int width, IN int height)
{
	m_areaWidth = width;
	m_areaHeight = height;
}

void LMineSet::SetNum(IN int num)
{
	m_mineNum = num;
}

void LMineSet::Init()
{
	for (int i = 0; i < 4; i++)
	{
		m_mineVBLocal[i] = g_mine[i];
	}

	m_mineShape.Scale = 2;

	m_minePosList = new LPoint[m_mineNum];
	for (int i = 0; i < m_mineNum; i++)
	{
		float x = (float)RandInt(0, m_areaWidth);
		float y = (float)RandInt(0, m_areaHeight);

		m_minePosList[i].X = x;
		m_minePosList[i].Y = y;
	}
}

LPoint* LMineSet::GetPosList()
{
	return m_minePosList;
}

void LMineSet::DrawTo(IN HDC hdc)
{
	for (int i = 0; i < m_mineNum; i++)
	{
		m_mineShape.PosX = m_minePosList[i].X;
		m_mineShape.PosY = m_minePosList[i].Y;
		m_mineShape.WorldTransform(m_mineVBLocal, 4, m_mineVBWorld);

		LPoint& pointStart = m_mineVBWorld[0];
		MoveToEx(hdc, (int)pointStart.X, (int)pointStart.Y, NULL);
		for (unsigned int vert = 1; vert < 4; ++vert)
		{
			LPoint& pointWorld = m_mineVBWorld[vert];

			LineTo(hdc, (int)pointWorld.X, (int)pointWorld.Y);
		}
		LineTo(hdc, (int)pointStart.X, (int)pointStart.Y);

	}

}