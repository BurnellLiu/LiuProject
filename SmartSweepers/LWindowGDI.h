
#ifndef _LWINDOWGDI_H_
#define _LWINDOWGDI_H_

#include <string>
using std::string;

#include <Windows.h>

//#include "LMacroDefine.h"

/// @brief Ӧ�ó��������
///
/// 
class LApParam
{
public:
	/// @brief ��ȡӦ�ó���ʵ�����
	/// @return Ӧ�ó���ʵ�����
	static HINSTANCE GetInstance()
	{
		return m_hInstance;
	}

	/// @brief ��ȡӦ�ó��������в���
	/// @param[in] cmdLine �����в���
	static void GetCmdLine(OUT string& cmdLine)
	{
		cmdLine.clear();
		cmdLine.append(m_cmdLine);
	}

	/// @brief ��ȡӦ�ó�����ʾ����
	/// @return Ӧ�ó�����ʾ����
	static int GetCmdShow()
	{
		return m_cmdShow;
	}

private:
	static HINSTANCE m_hInstance;  ///< Ӧ�ó���ʵ�����
	static string m_cmdLine;  ///< �����в���
	static int m_cmdShow;  ///< ��ʾ����

	friend int WINAPI WinMain(IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, 
		IN LPSTR lpCmdLine, IN int nShowCmd );
};

/// @brief �Զ��崰�ڻ���
/// 
/// 
class LWindow
{
public:
	LWindow();
	virtual ~LWindow();
   
	/// @brief ���ô����Ƿ���Ըı��С
	/// @param[in] bRet true(���Ըı��С) false(�����Ըı��С)
	void SetSizingBorder(IN bool bRet);

    /// @brief ���ô����Ƿ���ʾ����
    /// @param[in] bRet true(��ʾ����) false(����ʾ����)
    void SetShowCaption(IN bool bRet);

    /// @brief ���ô����Ƿ���ʾϵͳ�˵�
    ///
    /// ������ʾϵͳ�˵�����ͬʱ������ʾ����
    /// @param[in] bRet true(��ʾϵͳ�˵�) false(����ʾϵͳ�˵�)
    void SetShowSysMenu(IN bool bRet);

	/// @brief ���ô�����ʾ��С����ť
    ///
    /// ������ʾ��С����ť����ͬʱ������ʾϵͳ�˵�
	/// @param[in] bRet true(��ʾ��С����ť) false(����ʾ��С����ť)
	void SetShowMinimizeBox(IN bool bRet);

	/// @brief ���ô�����ʾ��󻯰�ť
    ///
    /// ������ʾ��󻯰�ť����ͬʱ������ʾϵͳ�˵�
	/// @param[in] bRet true(��ʾ��󻯰�ť) false(����ʾ��󻯰�ť)
	void SetShowMaximizeBox(IN bool bRet);

    /// @brief ���ô���λ��(Ĭ�Ͼ�����ʾ)
    /// @param[in] x X��
    /// @param[in] y Y��
    void SetPosition(IN int x, IN int y);

	/// @brief ���ô��ڴ�С
	/// @param[in] width ���
	/// @param[in] height �߶�
	void SetSize(IN int width, IN int height);

	/// @brief ���ô��ڱ���
	/// @param[in] title ����
	void SetTitle(IN const string& title);

	/// @brief ��ȡ���ڿ��
	/// @return ���ڿ��
	int GetWidth();

	/// @brief ��ȡ���ڸ߶�
	/// @return ���ڸ߶�
	int GetHeight();

    /// @brief ��ȡ�ͻ������ڿ��
    /// @return �ͻ������
    int GetClientWidth();

    /// @brief ��ȡ�ͻ������ڸ߶�
    /// @return �ͻ����߶�
    int GetClientHeight();

	/// @brief ��ȡ���ھ��
	/// @return ���ھ��
	HWND GetWndHandle();

	/// @brief ��ʾ����
	void Show();

	/// @brief ������Ϣѭ��
	///
	/// ֻ����������LMain�����вſɵ��øú���
	virtual void Exe();

private:
	/// @brief ��������
	///
	/// �÷����ᱻShow()�ڵ�һ�α�����ʱ����
	void Create();

	/// @brief �����Լ�����Ϣ������
	///
	/// �÷������ܲ���WM_CREATE��Ϣ
	/// @param[in] message
	/// @param[in] wParam
	/// @param[in] lParam
	/// @return 
	virtual LRESULT MessageProc(IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

	/// @brief ���ڹ��̻ص�����
	/// @param[in] hwnd ���ھ��
	/// @param[in] message
	/// @param[in] wParam
	/// @param[in] lParam
	/// @return 
	static LRESULT CALLBACK WindowProc (IN HWND hwnd, IN UINT message, IN WPARAM wParam, IN LPARAM lParam);

	LWindow(const LWindow&);
	void operator = (const LWindow&);

private:
	static int m_wndCount; ///< ���ڼ���

private:
	WNDCLASSEX m_wndClassEx; ///< ������ṹ
	HWND m_hWnd;  ///< ���ھ��

	bool m_bWndHadCreated; ///< �����Ƿ񱻴���
	string m_wndClassName; ///< ����������
	string m_wndTitle; ///< ���ڱ���
	int m_wndHeight; ///< ���ڸ߶�
	int m_wndWidth; ///< ���ڿ��

    bool m_bUseDefaultPos; ///< �����Ƿ�ʹ��Ĭ��λ��(����)
    int m_wndPosX; ///< ����λ��X����
    int m_wndPosY; ///< ����λ��Y����

	DWORD m_windowStyle; ///< ���ڷ��
};

/// @brief ���ƽӿ�
class IPaint
{
public:
	/// @brief ����
	/// @param[in] hdc ��ָ����hdc�л���
	virtual void Paint(IN HDC hdc) = 0;
};

/// @�󱸻�����DC��
class LBackBufferDC
{
public:
	LBackBufferDC();
	~LBackBufferDC();

	/// @brief ��ʼ���󱸻�����
	/// @param[in] hWnd ���ھ��
	/// @param[in] width �󱸻��������
	/// @param[in] height �󱸻������߶�
	void Init(IN HWND hWnd, IN int width, IN int height);

	/// @brief ��ָ��������ɫ������󱸻�����
	/// @param[in] red
	/// @param[in] green
	/// @param[in] blue
	/// @return ���󷵻�false
	bool Clear(BYTE red, BYTE green, BYTE blue);

	/// @brief �ں󱸻������л���ͼ��
	/// @param[in] pDrawObject ���ƵĶ���
	void Draw(IPaint* pDrawObject);

	/// @brief ���ƺ󱸻������е����ݵ�ǰ̨������
	void CopyToFrontDC();

	/// @brief ��ȡ�󱸻������豸��������
	/// @return �󱸻������豸��������
	HDC GetBackDC();

private:
	HWND m_hWnd; ///< ���ھ��
	int m_width; ///< �󱸻��������
	int m_height; ///< �󱸻������߶�
	HDC m_hBackDC; ///< �󱸻������豸��������
	HBITMAP m_hBitMap; ///< �󱸻�����ʹ�õ�λͼ
	HBITMAP m_hOldBitMap; ///< �󱸻�����ԭʼλͼ
};

/// @brief ֡��ʱ����
/// 
/// 
class LFrameTimer
{
public:
	/// @brief ���캯��
	/// @param[in] fps ÿ��֡��
	explicit LFrameTimer(IN int fps);
	~LFrameTimer();

	/// @brief ��ʼ��ʱ
	void Start();

	/// @brief �Ƿ񵽴���һ֡ʱ��
	/// @return true(����), false(δ����)
	bool ReadyForNextFrame();

	/// @brief �Ƿ񵽴���һ֡ʱ��
	///
	/// �÷�����׷��
	/// @return true(����), false(δ����)
	bool ReadyForNextFrameEx();

private:
	int m_fps; ///< ÿ��֡��
	LONGLONG m_performanceFreq; ///< CPUʱ��Ƶ��(��ÿ���ӵĵδ���)
	LONGLONG m_frameTime; ///< ÿ֡�ĵδ���
	LONGLONG m_lastTime; ///< �ϴ�ʱ��
	LONGLONG m_currentTime; ///< ��ǰʱ��
	
	
};
#endif