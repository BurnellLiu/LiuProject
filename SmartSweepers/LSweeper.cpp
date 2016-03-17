#include "LSweeper.h"

#include "LLib.h"

LPoint LSweeper::s_sweeperVBLocal[16] =
{
	// ����
	LPoint(-1, -1),
	LPoint(-1, 1),
	LPoint(-0.5, 1),
	LPoint(-0.5, -1),

	// ����
	LPoint(0.5, -1),
	LPoint(1, -1),
	LPoint(1, 1),
	LPoint(0.5, 1),

	LPoint(-0.5, -0.5),
	LPoint(0.5, -0.5),

	LPoint(-0.5, 0.5),
	LPoint(-0.25, 0.5),
	LPoint(-0.25, 1.75),
	LPoint(0.25, 1.75),
	LPoint(0.25, 0.5),
	LPoint(0.5, 0.5)
};

LPoint LSweeper::s_sweeperVBWorld[16];

LSweeper::LSweeper()
{
	m_liveWidth = 0.0f;
	m_liveHeight =0.0f;
	m_score = 0;
}

LSweeper::~LSweeper()
{

}

void LSweeper::SetLiveArea(IN float width, IN float height)
{
	m_liveWidth = width;
	m_liveHeight = height;
}

void LSweeper::Init()
{
	m_sweeperShape.PosX = (float)LRandom::RandInt(0, (int)m_liveWidth);
	m_sweeperShape.PosY = (float)LRandom::RandInt(0, (int)m_liveHeight);
	m_sweeperShape.Scale = 6;
	m_vLookAt.X = 0.0;
	m_vLookAt.Y = 1.0;
	
	m_neuronNet.Init(1, 2, 1, 10);

	m_inputParamList.Reset(1);
	m_outputParamList.Reset(2);
}

void LSweeper::Update(IN LPoint* minePosList, IN int mineNum)
{
	/*
	if (KEYDOWN('W'))
	{
		m_sweeperShape.PosX += m_vLookAt.X * MAX_TRANSLATION_PER_TICK;
		m_sweeperShape.PosY += m_vLookAt.Y * MAX_TRANSLATION_PER_TICK;
	}

	if (KEYDOWN('S'))
	{
		m_sweeperShape.PosX -= m_vLookAt.X * MAX_TRANSLATION_PER_TICK;
		m_sweeperShape.PosY -= m_vLookAt.Y * MAX_TRANSLATION_PER_TICK;
	}

	// ��ʱ����ת�Ƕ�
	if (KEYDOWN('A'))
	{
		m_sweeperShape.Rotation -= MAX_ROTATION_PER_TICK;
		if (m_sweeperShape.Rotation < (-2 * PI))
		{
			m_sweeperShape.Rotation += 2 * PI;
		}

		m_vLookAt.X = -sin(m_sweeperShape.Rotation);
		m_vLookAt.Y = cos(m_sweeperShape.Rotation);
	}

	// ˳ʱ����ת�Ƕ�
	if (KEYDOWN('D'))
	{
		m_sweeperShape.Rotation += MAX_ROTATION_PER_TICK;
		if (m_sweeperShape.Rotation > (2 * PI))
		{
			m_sweeperShape.Rotation -= 2 * PI;
		}
		m_vLookAt.X = -sin(m_sweeperShape.Rotation);
		m_vLookAt.Y = cos(m_sweeperShape.Rotation);
	}
	*/
	// �ҳ����Լ�����ĵ���
	float nearestMineDis = 999999999.0f;
	float nearestMineX; // �������X����
	float nearestMineY; // �������Y����
	unsigned int nearestMineIndex; // �����������
	for (int i = 0 ; i < mineNum; i++)
	{
		LPoint& minePos = minePosList[i];
		float disX = m_sweeperShape.PosX - minePos.X;
		float disY = m_sweeperShape.PosY - minePos.Y;
		float dis = disX * disX + disY * disY;
		if (dis < nearestMineDis)
		{
			nearestMineDis = dis;
			nearestMineX = minePos.X;
			nearestMineY = minePos.Y;
			nearestMineIndex = i;
		}


	}

	// ����Ŀ�귽��
	LVector2D target;
	target.X = nearestMineX - m_sweeperShape.PosX;
	target.Y = nearestMineY - m_sweeperShape.PosY;
	target.Normalize();

	// ��Ŀ�귽�������߷�������������
	/*
	m_inputParam[0] = target.X;
	m_inputParam[1] = target.Y;
	m_inputParam[2] = m_vLookAt.X;
	m_inputParam[3] = m_vLookAt.Y;
	*/
	m_inputParamList.Data[0] = target.Dot(m_vLookAt) * target.Sign(m_vLookAt);
	m_neuronNet.Active(m_inputParamList, m_outputParamList);

	// ��ȡ������ƫת��С
	float lTrack = m_outputParamList.Data[0]; 
	float rTrack = m_outputParamList.Data[1];

	float rotForce = lTrack - rTrack;
	
	if (rotForce > 0.3f)
		rotForce = 0.3f;
	if (rotForce < -0.3f)
		rotForce = -0.3f;

	// ���½Ƕ�
	m_sweeperShape.Rotation += rotForce;

	// ��ȡ�ٶ�
	float speed = (lTrack + rTrack);	

	// ������������
	m_vLookAt.X = -sin(m_sweeperShape.Rotation);
	m_vLookAt.Y = cos(m_sweeperShape.Rotation);

	// ����λ��
	m_sweeperShape.PosX += (m_vLookAt.X * speed);
	m_sweeperShape.PosY += (m_vLookAt.Y * speed);

	// �����߽�
	if (m_sweeperShape.PosX < 0.0f)
		m_sweeperShape.PosX = m_liveWidth;
	if (m_sweeperShape.PosX > m_liveWidth)
		m_sweeperShape.PosX = 0.0f;
	if (m_sweeperShape.PosY < 0.0f)
		m_sweeperShape.PosY = m_liveHeight;
	if (m_sweeperShape.PosY > m_liveHeight)
		m_sweeperShape.PosY = 0.0f;

	// �ж��Ƿ��ҵ�����
	float disX = m_sweeperShape.PosX - nearestMineX;
	float disY = m_sweeperShape.PosY - nearestMineY;
	float dis = disX * disX + disY * disY;
	if (dis < 50.0f) // С�ڵ���7�����ж��ҵ�
	{
		m_score++;

		// ���·��õ���
		minePosList[nearestMineIndex].X = (float)LRandom::RandInt(0, (int)m_liveWidth);
		minePosList[nearestMineIndex].Y = (float)LRandom::RandInt(0, (int)m_liveHeight);
	}

}



void LSweeper::Paint(IN HDC hdc)
{
	/*
	wstring strGeneration;
	strGeneration += IntToWStr(m_score);
	SetBkMode(hdc, TRANSPARENT); // �����ı�����͸��
	TextOutW(hdc, (int)m_sweeperShape.PosX-5, (int)m_sweeperShape.PosY-5, strGeneration.c_str(), strGeneration.length());
	SetBkMode(hdc, OPAQUE); // �����ı�������͸��
	*/

	m_sweeperShape.WorldTransform(s_sweeperVBLocal, 16, s_sweeperVBWorld);

	// ��������
	MoveToEx(hdc, (int)s_sweeperVBWorld[0].X, (int)s_sweeperVBWorld[0].Y, NULL);
	for (int i =1; i < 4; ++i)
	{
		LineTo(hdc, (int)s_sweeperVBWorld[i].X, (int)s_sweeperVBWorld[i].Y);
	}
	LineTo(hdc, (int)s_sweeperVBWorld[0].X, (int)s_sweeperVBWorld[0].Y);

	//��������
	MoveToEx(hdc, (int)s_sweeperVBWorld[4].X, (int)s_sweeperVBWorld[4].Y, NULL);
	for (int i = 5; i < 8; ++i)
	{
		LineTo(hdc, (int)s_sweeperVBWorld[i].X, (int)s_sweeperVBWorld[i].Y);
	}
	LineTo(hdc, (int)s_sweeperVBWorld[4].X, (int)s_sweeperVBWorld[4].Y);

	MoveToEx(hdc, (int)s_sweeperVBWorld[8].X, (int)s_sweeperVBWorld[8].Y, NULL);
	LineTo(hdc, (int)s_sweeperVBWorld[9].X, (int)s_sweeperVBWorld[9].Y);

	MoveToEx(hdc, (int)s_sweeperVBWorld[10].X, (int)s_sweeperVBWorld[10].Y, NULL);
	for (int i = 11; i < 16; ++i)
	{
		LineTo(hdc, (int)s_sweeperVBWorld[i].X, (int)s_sweeperVBWorld[i].Y);
	}
}

int LSweeper::GetScore()
{
	return m_score;
}

void LSweeper::ResetScore()
{
	m_score = 0;
}

void LSweeper::SetNNWeight(IN LNNWeightList& weightList)
{
	m_neuronNet.PutWeightTotal(weightList);
}

void LSweeper::GetNNSplitPointList(INOUT LNNSplitPointList& splitPointList)
{
	m_neuronNet.GetSplitPointList(splitPointList);
}

int LSweeper::GetNNWeightNum()
{
	LNNWeightList weightList;
	m_neuronNet.GetWeightTotal(weightList);
	return weightList.Length;
}