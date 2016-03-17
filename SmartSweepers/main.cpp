
#include "main.h"

#include <stdio.h>

#include "LLib.h"


LGameWindow::LGameWindow()
{
	m_sweeperNum = 0;
	m_mineNum = 0;
	m_frameCount = 0;
	m_generationCount = 0;
	m_bFastGenerate = false;
	m_sweeperList = NULL;

	m_hOldPen = 0;
	m_hRedPen = 0;
	m_hGreenPen = 0;
	m_hBluePen = 0;

	m_weightListLen = 0;
}

LGameWindow::~LGameWindow()
{
	this->CleanUp();
}

void LGameWindow::InitGame()
{
	this->CleanUp();

	int width = GetWidth();
	int height = GetHeight();

	// 初始化背景缓冲区
	HWND hWnd = this->GetWndHandle();
	m_backDC.Init(hWnd, width, height);

	m_hBluePen  = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_hRedPen   = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 150, 0));
	m_hOldPen	= NULL;

	LRandom::SRandTime();

	m_sweeperNum = 30;
	m_sweeperList = new LSweeper[m_sweeperNum];
	// 初始化扫雷机集合
	for (int i = 0; i < m_sweeperNum; i++)
	{
		m_sweeperList[i].SetLiveArea((float)width, (float)height);
		m_sweeperList[i].Init();
	}

	
	// 初始化地雷集合
	m_mineNum = 50;
	m_mineSet.SetNum(m_mineNum);
	m_mineSet.SetArea(width, height);
	m_mineSet.Init();

	// 初始化遗传算法
	m_geneticOptimize.SetMutateProb(0.2f);
	m_geneticOptimize.SetPopSize(m_sweeperNum);
	m_geneticOptimize.SetMutateStep(100);

	LNNSplitPointList splitPointList;
	m_sweeperList[0].GetNNSplitPointList(splitPointList);
	m_geneticOptimize.SetCrossOverSplitPointList(splitPointList);

	int geneListLength = m_sweeperList[0].GetNNWeightNum();
	LOGenomeDomain domainList;
	domainList.Reset(geneListLength);

	for (int i = 0; i < geneListLength; i++)
	{
		domainList.Data[i].Min = -1000;
		domainList.Data[i].Max = 1000;
	}
	m_geneticOptimize.Init(NULL, domainList);


	// 更新所有扫雷机的神经网络权重
	m_weightListLen = geneListLength;
	m_weightList.Reset(m_weightListLen);
	for (int i = 0; i < m_sweeperNum; i++)
	{
		LOSolution& genome = m_geneticOptimize.GetSolution(i);
		for (int j = 0; j < genome.PGenome->Length; j++)
		{
			m_weightList.Data[j] = (float)genome.PGenome->Data[j]/1000.0f;
		}

		m_sweeperList[i].SetNNWeight(m_weightList);
	}
	
}

void LGameWindow::RunGame()
{
	m_frameCount++;

	// 更新所有扫雷机
	for (int i = 0; i < m_sweeperNum; i++)
	{
		m_sweeperList[i].Update(m_mineSet.GetPosList(), m_mineNum);
	}

	if (m_frameCount == 3600)// 每经过3600帧进化一次
	{
		m_generationCount++;
		m_frameCount = 0;

		// 使用扫雷机得分来更新种群中每个个体的成本
		int bestScore = 0;
		int totalScore = 0;
		for (int i = 0; i < m_sweeperNum; i++)
		{
			LOSolution& genome = m_geneticOptimize.GetSolution(i);
			int score = m_sweeperList[i].GetScore();
			genome.Cost -= score;
			if (score > bestScore)
				bestScore = score;
			totalScore += score;

			m_sweeperList[i].ResetScore();
		}
		m_bestSocreList.push_back(bestScore);
		m_avgSocoreList.push_back(totalScore/m_sweeperNum);

		// 繁殖一代
		m_geneticOptimize.Breed();

		// 使用新一代的权重来更新神经网络
		for (int i = 0; i < m_sweeperNum; i++)
		{
			LOSolution& genome = m_geneticOptimize.GetSolution(i);
			for (int j = 0; j < genome.PGenome->Length; j++)
			{
				m_weightList.Data[j] = (float)genome.PGenome->Data[j]/1000.0f;
			}

			m_sweeperList[i].SetNNWeight(m_weightList);
		}

	}
}

void LGameWindow::PaintGame()
{
	m_backDC.Clear(255, 255, 255);

	HDC hBackDC = m_backDC.GetBackDC();

	if (!m_bFastGenerate)
	{
		// 绘制所有扫雷机
		m_hOldPen = (HPEN)SelectObject(hBackDC, m_hRedPen);
		for (int i = 0; i < m_sweeperNum && i < 6; i++)
		{
			m_backDC.Draw((IPaint*)&m_sweeperList[i]);
		}

		SelectObject(hBackDC, m_hBluePen);
		for (int i = 6; i < m_sweeperNum; i++)
		{
			m_backDC.Draw((IPaint*)&m_sweeperList[i]);
		}

		// 绘制地雷集合
		SelectObject(hBackDC, m_hGreenPen);
		m_backDC.Draw((IPaint*)&m_mineSet);
		SelectObject(hBackDC, m_hOldPen);
	}
	else
	{
		PaintLineChart(hBackDC);
	}
	
	
	// 绘制遗传的代数
	char strGeneration[256];
	int length = sprintf_s(strGeneration, 256, "Generation: %d", m_generationCount);
	SetBkMode(hBackDC, TRANSPARENT); // 设置文本背景透明
	TextOutA(hBackDC, 10, GetHeight() - 20, strGeneration, length);
	SetBkMode(hBackDC, OPAQUE); // 设置文本背景不透明
	
	m_backDC.CopyToFrontDC();
}

void LGameWindow::PaintLineChart(HDC hdc)
{
	static int s_bestScore = 0;

	int width = GetWidth();
	int height = GetHeight();

	int currentBestIndex = m_bestSocreList.size() - 1;
	int currentBestScore = 0;
	if (currentBestIndex >= 0)
	{
		currentBestScore = m_bestSocreList[currentBestIndex];
		if (currentBestScore > s_bestScore)
			s_bestScore = currentBestScore;
	}

	float hSlice = (float)width/(m_generationCount + 1);
	float vSlice = (float)height/((s_bestScore + 1) * 1.3f);

	char strCurrentBestScore[256];
	int length = sprintf_s(strCurrentBestScore, 256, "Current Best Score: %d", currentBestScore);
	SetBkMode(hdc, TRANSPARENT); // 设置文本背景透明
	TextOutA(hdc, 10, 20, strCurrentBestScore, length);
	SetBkMode(hdc, OPAQUE); // 设置文本背景不透明

	SelectObject(hdc, m_hRedPen);
	float x = 0;
	MoveToEx(hdc, 0, height, NULL);
	for (unsigned int i = 0; i < m_bestSocreList.size(); ++i)
	{
		x += hSlice;
		LineTo(hdc, (int)x, int(height - vSlice * m_bestSocreList[i]));
	}

	int currentAvgIndex = m_avgSocoreList.size() - 1;
	int currentAvgScore = 0;
	if (currentBestIndex >= 0)
	{
		currentAvgScore = m_avgSocoreList[currentAvgIndex];
	}
	char strCurrentAvgScore[256];
	length = sprintf_s(strCurrentAvgScore, 256, "Current Avg Score: %d", currentAvgScore);
	SetBkMode(hdc, TRANSPARENT); // 设置文本背景透明
	TextOutA(hdc, 10, 40, strCurrentAvgScore, length);
	SetBkMode(hdc, OPAQUE); // 设置文本背景不透明

	SelectObject(hdc, m_hBluePen);
	x = 0;
	MoveToEx(hdc, 0, height, NULL);
	for (unsigned int i = 0; i < m_avgSocoreList.size(); ++i)
	{
		x += hSlice;
		LineTo(hdc, (int)x, (int)(height - vSlice * m_avgSocoreList[i]));
	}
	SelectObject(hdc, m_hOldPen);

}

void LGameWindow::CleanUp()
{
	LDestroy::SafeDeleteArray(m_sweeperList);
	SAFE_DELETE_GDI(m_hRedPen);
	SAFE_DELETE_GDI(m_hBluePen);
	SAFE_DELETE_GDI(m_hGreenPen);
}

void LGameWindow::Exe()
{
	this->InitGame();

	bool bDone = false;
	MSG msg;

	LFrameTimer frameTimer(60);
	frameTimer.Start();

	while(!bDone)
	{
		while(PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
		{
			if( msg.message == WM_QUIT ) 
			{
				bDone = true;
			} 
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if (frameTimer.ReadyForNextFrameEx() || m_bFastGenerate)
		{
			this->RunGame();

			if (m_bFastGenerate && (m_frameCount != 0))
				continue;

			InvalidateRect(GetWndHandle(), NULL, TRUE);
			UpdateWindow(GetWndHandle());
		}
		else
		{
			Sleep(0);
		}

	}
}

void LGameWindow::KeyUpMessage(IN WPARAM wParam, IN LPARAM lParam)
{
	switch(wParam)
	{
	case 'F':
		m_bFastGenerate = !m_bFastGenerate;
		break;
	default:
		break;
	}
}

LRESULT LGameWindow::MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam)
{
	switch (message)
	{	
	case WM_DESTROY:
		PostQuitMessage (0);
		return 0;
	case WM_SIZE:
		{
			// 窗口大小改变重新初始化后备缓冲区
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			this->SetSize(width, height);
			HWND hWnd = this->GetWndHandle();
			m_backDC.Init(hWnd, width, height);
		}
		break;
	case WM_PAINT:
		this->PaintGame();
		break;
	case WM_KEYUP:
		this->KeyUpMessage(wParam, lParam);
		break;
	default:
		break;
	}

	return DefWindowProc(GetWndHandle(), message, wParam, lParam);
}

int LMain()
{
	LGameWindow mainWnd;
	//mainWnd.SetSize(500, 500);
	mainWnd.Show();
	mainWnd.Exe();

	return 0;
}