
#include "LWebcam.h"

#include <sstream>
using std::stringstream;

#include <Windows.h>
#include <DShow.h>

#include <objbase.h>

#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")

/// <SUMMARY>
/// ��ȫ�ͷź�
/// </SUMMARY>
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)\
do\
{\
    if (p)\
    {\
        p->Release();\
        p = NULL;\
    }\
}while(0)
#endif


/// <SUMMARY>
/// ��ʼ��COM��(���߳��׼�)
/// </SUMMARY>
class CComInitS
{
public:
    CComInitS()
        : m_success(false)
    {
        // CoInitialize���ú󷵻�S_FALSE��S_OK�������CoUninitialize
        // CoInitialize���ú󷵻�RPC_E_CHANGE_MODE, ������ǰ�߳��ѱ���ʼ��(�Һ͵�ǰģʽ��ͬ),����Ҫ����CoUninitialize
        HRESULT hr = CoInitialize(0);
        if (hr == S_FALSE || hr == S_OK)
        {
            m_success = true;
        }
    }

    virtual ~CComInitS()
    {
        if (m_success)
            CoUninitialize();
    }
private:
    bool m_success; // ��ʶCOM���Ƿ��ʼ���ɹ�
    CComInitS(const CComInitS&);
    CComInitS& operator = (const CComInitS&);
};

/// <SUMMARY>
/// ��ʼ��COM��(���߳��׼�)
/// </SUMMARY>
class CComInitM
{
public:
    CComInitM()
        : m_success(false)
    {
        // CoInitializeEx���ú󷵻�S_FALSE��S_OK�������CoUninitialize
        // CoInitializeEx���ú󷵻�RPC_E_CHANGE_MODE, ������ǰ�߳��ѱ���ʼ��(�Һ͵�ǰģʽ��ͬ),����Ҫ����CoUninitialize
        HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (hr == S_FALSE || hr == S_OK)
        {
            m_success = true;
        }
    }

    virtual ~CComInitM()
    {
        if (m_success)
            CoUninitialize();
    }
private:
    bool m_success; // ��ʶCOM���Ƿ��ʼ���ɹ�
    CComInitM(const CComInitM&);
    CComInitM& operator = (const CComInitM);
};


CDBIImage::CDBIImage()
    : Size(0), Data(0)
{

}
CDBIImage::~CDBIImage()
{
    if (Data != 0)
    {
        delete[] Data;
        Data = 0;
    }
    Size = 0;
}

bool CDBIImage::GetAvgLuma(OUT int& avgLuma)
{
    if (this->Size == 0)
        return false;

    LPBITMAPINFOHEADER pInfoHeader = (LPBITMAPINFOHEADER)this->Data;

    int width = pInfoHeader->biWidth;
    int height = pInfoHeader->biHeight;

    //���洢ͼ������ÿ���ֽ���Ϊ4�ı���
    int lineByte=(width * (pInfoHeader->biBitCount/8) + 3)/4 * 4; // ÿ�е��ֽ���

    unsigned char* pImageData = this->Data + pInfoHeader->biSize;
    unsigned long lumaSum = 0;

    // 32λͼ
    if (pInfoHeader->biBitCount == 32)
    {
        for (int i = 0; i < height; i++)
        {
            int heightOffset = i * lineByte;
            for (int j = 0; j < width; j++)
            {
                RGBQUAD* pRgbQuad = (RGBQUAD*)(pImageData + heightOffset + j * sizeof(RGBQUAD));
                unsigned long r = pRgbQuad->rgbRed;
                unsigned long g = pRgbQuad->rgbGreen;
                unsigned long b = pRgbQuad->rgbBlue;

                unsigned char luma = (unsigned char)(((66 * r + 129 * g + 25 * b + 128) >> 8) + 16);
                lumaSum += luma;
            }
        }

        avgLuma = lumaSum / (width * height);
        return true;

    }


    return false;
}
bool CDBIImage::SaveToBMP(IN const char* fileName)
{
    if (this->Size == 0)
        return false;

    LPBITMAPINFOHEADER pInfoHeader = (LPBITMAPINFOHEADER)this->Data;
    BITMAPFILEHEADER fileHeader;

    fileHeader.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M"
    // ���������ļ��Ĵ�С
    fileHeader.bfSize =(DWORD)
        (sizeof(BITMAPFILEHEADER) + // �ļ�ͷ��С
        pInfoHeader->biSize + // ��Ϣͷ��С
        pInfoHeader->biClrUsed * sizeof(RGBQUAD) + //��ɫ���С, С��24λͼ�õ���ɫ��
        pInfoHeader->biSizeImage); // ͼ�����ݴ�С
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;

    // ����ͼ�����ݵ�ƫ��
    fileHeader.bfOffBits = 
        (DWORD)sizeof(BITMAPFILEHEADER) + // �ļ�ͷ��С
        pInfoHeader->biSize +  // ��Ϣͷ��С
        pInfoHeader->biClrUsed * sizeof (RGBQUAD); // ��ɫ���С

    DWORD	dwWritten;
    HANDLE hFile = CreateFileA(
        fileName, 
        GENERIC_WRITE, 
        FILE_SHARE_READ, 
        NULL,
        CREATE_ALWAYS, 
        FILE_ATTRIBUTE_NORMAL, 
        0);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    WriteFile(hFile, (LPVOID)&fileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL);
    WriteFile(hFile, (LPCVOID)pInfoHeader , this->Size, &dwWritten, 0);
    CloseHandle(hFile);

    return true;
}

/// <SUMMARY>
/// ����ͷʵ����
/// </SUMMARY>
class CWebcam : public LWebcam
{
public:
    /// <SUMMARY>
    /// ���캯��
    /// </SUMMARY>
    /// <PARAM name = "friendlyName" dir = "IN">
    /// ����ͷ�Ѻ�����
    /// </PARAM>
    /// <PARAM name = "pCaptureFilter" dir = "IN">
    /// ��Ƶ�ɼ�������ָ��
    /// </PARAM>
    CWebcam(IN const wstring& friendlyName, IN const wstring& displayName, IN IBaseFilter* pCaptureFilter)
        : m_friendlyName(friendlyName), m_displayName(displayName), m_pCaptureFilter(pCaptureFilter)
    {
        m_pRenderFilter = NULL;
        m_pSmartTeeFilter = NULL;

        m_pGraphBuilder = NULL;
        m_pCaptureGraphBuilder = NULL;
        m_pMediaControl = NULL;
        m_pMediaEvent = NULL;
        m_pBasicVideo = NULL;

        m_pQualProp = NULL;

        m_bInitSuccess = false;
    }

    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    ~CWebcam()
    {
        SAFE_RELEASE(m_pQualProp);
        SAFE_RELEASE(m_pBasicVideo);
        SAFE_RELEASE(m_pRenderFilter);
        SAFE_RELEASE(m_pSmartTeeFilter);
        SAFE_RELEASE(m_pCaptureFilter);
        SAFE_RELEASE(m_pMediaControl);
        SAFE_RELEASE(m_pMediaEvent);
        SAFE_RELEASE(m_pVideoWindow);
        SAFE_RELEASE(m_pGraphBuilder);
        SAFE_RELEASE(m_pCaptureGraphBuilder);
    }

    /// <SUMMARY>
    /// ��ʼ��
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    bool Init()
    {
        bool bRet = false;

        IPin* pCaptureFilterOut = NULL;
        IPin* pSmartTeeFilterIn = NULL;
        IPin* pSmartTeeFilterPreview = NULL;
        IPin* pRenderIn = NULL;

        HRESULT hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, 
            CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&m_pCaptureGraphBuilder);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Create IID_ICaptureGraphBuilder2 Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGraphBuilder);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Create IID_IGraphBuilder Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = CoCreateInstance(CLSID_SmartTee,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pSmartTeeFilter);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Create CLSID_SmartTee Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        // ʹ��CLSID_VideoRenderer���ܵ��²��ܽ�ͼ, ����ʹ��CLSID_VideoMixingRenderer
        hr = CoCreateInstance(CLSID_VideoMixingRenderer,NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pRenderFilter); 
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Create CLSID_VideoMixingRenderer Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->AddFilter(m_pCaptureFilter, L"Capture Filter");
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "AddFilter Capture Filter Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->AddFilter(m_pSmartTeeFilter,L"SmartTee Filter");
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "AddFilter SmartTee Filter Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }
        hr = m_pGraphBuilder->AddFilter(m_pRenderFilter,L"Render Filter");
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "AddFilter Render Filter Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pCaptureGraphBuilder->SetFiltergraph(m_pGraphBuilder);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "SetFiltergraph Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }


        hr = m_pCaptureGraphBuilder->FindPin(m_pCaptureFilter, PINDIR_OUTPUT, &PIN_CATEGORY_CAPTURE, 
            NULL, FALSE, 0, &pCaptureFilterOut);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Find Capture Filter Output Pin Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }


        hr = m_pSmartTeeFilter->FindPin(L"Input", &pSmartTeeFilterIn);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Find Smart Tee Filter Input Pin Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->Connect(pCaptureFilterOut, pSmartTeeFilterIn);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Connect Capture Filter Output Pin And Smart Tee Filter Input Pin Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }


        hr = m_pSmartTeeFilter->FindPin(L"Preview",&pSmartTeeFilterPreview);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Find Smart Tee Filter Preview Pin Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pCaptureGraphBuilder->FindPin(m_pRenderFilter, PINDIR_INPUT, 
            NULL, NULL, FALSE, 0, &pRenderIn);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Find Render Filter Preview Pin Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->Connect(pSmartTeeFilterPreview, pRenderIn);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "Connect Smart Tee Filter Output Pin And Render Filter Input Pin Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_pMediaControl);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "QueryInterface IID_IMediaControl Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&m_pMediaEvent);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "QueryInterface IID_IMediaEvent Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&m_pVideoWindow);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "QueryInterface IID_IVideoWindow Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pGraphBuilder->QueryInterface(IID_IBasicVideo, (void**)&m_pBasicVideo);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "QueryInterface IID_IBasicVideo Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        hr = m_pRenderFilter->QueryInterface(IID_IQualProp, (void**)&m_pQualProp);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "QueryInterface IID_IQualProp Fail, Error Code: 0X" << std::hex << hr;
            m_bInitSuccess = false;
            bRet = false;
            goto SAFE_EXIT;
        }

        /*
        hr = m_pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pCaptureFilter, NULL, NULL);
        if (hr != S_OK && hr != VFW_S_NOPREVIEWPIN)
        {
        m_errStream.str("");
        m_errStream << "RenderStream Fail, Error Code: 0X" << std::hex << hr;
        m_bInitSuccess = false;
        bRet = false;
        goto SAFE_EXIT;
        }
        */

        m_bInitSuccess = true;
        bRet = true;

SAFE_EXIT:
        SAFE_RELEASE(pCaptureFilterOut);
        SAFE_RELEASE(pSmartTeeFilterIn);
        SAFE_RELEASE(pSmartTeeFilterPreview);
        SAFE_RELEASE(pRenderIn);

        return bRet;
    }

    bool SetParentWindow(IN void* hwnd, IN long left, IN long top, IN long width, IN long height)
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        HRESULT hr = m_pVideoWindow->put_Owner((OAHWND)hwnd);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "put_Owner Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }

        hr = m_pVideoWindow->put_WindowStyle(WS_CHILD);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "put_WindowStyle Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }



        hr = m_pVideoWindow->SetWindowPosition(left, top, width, height);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "SetWindowPosition Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }

        hr = m_pVideoWindow->put_MessageDrain((OAHWND)hwnd);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "put_MessageDrain Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }

        return true;
    }

    bool GetFriendlyName(OUT wstring& name)
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        name = m_friendlyName;

        return true;
    }

    bool GetDisplayName(OUT wstring& name)
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        name = m_displayName;

        return true;
    }

    bool GetCurrentImage(OUT CDBIImage& image)
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        if (image.Data != NULL)
            delete[] image.Data;

        image.Data = NULL;
        image.Size = 0;

        HRESULT hr = m_pBasicVideo->GetCurrentImage(&image.Size, NULL);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "GetCurrentImage Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }

        image.Data = new unsigned char[image.Size];
        hr = m_pBasicVideo->GetCurrentImage(&image.Size, (long*)image.Data);
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "GetCurrentImage Fail, Error Code: 0X" << std::hex << hr;
            delete[] image.Data;
            image.Data = 0;
            image.Size = 0;
            return false;
        }

        return true;
    }

    bool Run()
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        HRESULT hr = m_pMediaControl->Run();

        return true;
    }

    bool Pause()
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        HRESULT hr = m_pMediaControl->Pause();

        FILTER_STATE fs;
        do 
        {
            m_pMediaControl->GetState(1000, (OAFilterState*)&fs);
        } while (fs != State_Paused);

        return true;
    }

    bool Stop()
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }

        HRESULT hr = m_pMediaControl->Stop();
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "IMediaControl Stop Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }

        return true;
    }

    bool GetAvgFrameRate(int& rate)
    {
        if (!m_bInitSuccess)
        {
            m_errStream.str("");
            m_errStream << "Init Fail";
            return false;
        }
        HRESULT hr = m_pQualProp->get_AvgFrameRate(&rate);
        rate /= 100;
        if (hr != S_OK)
        {
            m_errStream.str("");
            m_errStream << "get_AvgFrameRate Fail, Error Code: 0X" << std::hex << hr;
            return false;
        }

        return true;
    }

    string GetErrorMessage()
    {
        return m_errStream.str();
    }

    void GetErrorMessage(string& error)
    {
        error = m_errStream.str();
    }

    void Release()
    {
        delete this;
    }

private:
    CComInitS comInitS;
    wstring m_friendlyName; // ����ͷ�Ѻ�����
    wstring m_displayName; // ��ʾ����

    IBaseFilter* m_pSmartTeeFilter; // Smart Tee Filter
    IBaseFilter* m_pRenderFilter; // ��Ƶ��Ⱦ������
    IBaseFilter* m_pCaptureFilter; // ��Ƶ�ɼ�������
    IGraphBuilder* m_pGraphBuilder; //������������
    ICaptureGraphBuilder2* m_pCaptureGraphBuilder; //��Ƶ�ɼ�������������

    IBasicVideo* m_pBasicVideo; // ��Ƶ�ӿ�
    IVideoWindow* m_pVideoWindow; // ���ڿ�����Ƶ���ڵ�����
    IMediaControl* m_pMediaControl; // �û�����ӿ�
    IMediaEvent* m_pMediaEvent; // �¼��ӿ�

    IQualProp* m_pQualProp; // ����ͷ�������Խӿ�

    bool m_bInitSuccess; // ��ʼ����ʶ��
    stringstream m_errStream; // ��������
};

/// <SUMMARY>
/// ö������ͷ
/// </SUMMARY>
/// <PARAM name = "webcamList" dir = "OUT">
/// �洢����ͷ����
/// </PARAM>
/// <PARAM name = "errorMessage" dir = "OUT">
/// �洢������Ϣ
/// </PARAM>
/// <RETURNS>
/// �ɹ�����true, ʧ�ܷ���false
/// </RETURNS>
bool LEnumWebcam(OUT vector<LWebcam*>& webcamList, string& errorMessage)
{
    CComInitS comInitS;
    stringstream errStream; // ��������

    errorMessage.clear();
    webcamList.clear();

    bool bRet = false;
    ICreateDevEnum* pDevEnumCreater = NULL;
    IEnumMoniker* pVideoEnumerator = NULL;

    HRESULT hr = CoCreateInstance(
        CLSID_SystemDeviceEnum,
        NULL, 
        CLSCTX_INPROC_SERVER, 
        IID_ICreateDevEnum, 
        (void**)&pDevEnumCreater);

    if (hr != S_OK)
    {
        errStream.str("");
        errStream << "Create ICreateDevEnum Fail, Error Code: " << std::hex << hr;
        bRet = false;
        goto SAFE_EXIT;
    }

    hr = pDevEnumCreater->CreateClassEnumerator(
        CLSID_VideoInputDeviceCategory, 
        &pVideoEnumerator, 
        0);

    if (hr != S_OK)
    {
        errStream.str("");
        errStream << "ICreateDevEnum CreateClassEnumerator Fail, Error Code: " << std::hex << hr;
        bRet = false;
        goto SAFE_EXIT;
    }

    while (true)
    {
        IMoniker* pVideo = NULL;
        ULONG ulFetched = 0;
        hr = pVideoEnumerator->Next(1, &pVideo, &ulFetched);

        if (hr != S_OK)
        {
            break;
        }

        wstring displayName;
        LPOLESTR strDisplayName = NULL;
        hr = pVideo->GetDisplayName(NULL, NULL, &strDisplayName);
        if (SUCCEEDED(hr))
        {
            displayName = strDisplayName;
            CoTaskMemFree(strDisplayName);
        }

        IPropertyBag* pPropertyBag = NULL;
        hr = pVideo->BindToStorage(
            NULL,
            NULL,
            IID_IPropertyBag,
            (void**)&pPropertyBag
            );

        if (hr != S_OK)
        {
            errStream.str("");
            errStream << "BindToStorage Fail, Error Code: " << std::hex << hr;
        }


        wstring wstrFriendlyName;
        if (pPropertyBag != NULL)
        {
            VARIANT variant;
            VariantInit(&variant);
            hr = pPropertyBag->Read(L"FriendlyName", &variant, NULL);
            if (hr != S_OK)
            {
                errStream.str("");
                errStream << "Read(FriendlyName Fail, Error Code: " << std::hex << hr;   
            }

            if (variant.vt != VT_EMPTY && variant.vt != VT_NULL)
            {
                wstrFriendlyName = variant.bstrVal;
            }
            VariantClear(&variant);

        }

        IBaseFilter* pCaptureFilter = NULL;
        hr = pVideo->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCaptureFilter);
        if (hr != S_OK)
        {
            errStream.str("");
            errStream << "BindToObject Fail, Error Code: " << std::hex << hr;
        }

        if (pCaptureFilter != NULL)
        {
            LWebcam* pWebcam = new CWebcam(wstrFriendlyName, displayName, pCaptureFilter);
            if (((CWebcam*)pWebcam)->Init())
            {
                webcamList.push_back(pWebcam);
            }
            else
            {
                errStream.str("");
                errStream << "IWebcam Init Fail, " << pWebcam->GetErrorMessage();
                pWebcam->Release();
            }
        }


        SAFE_RELEASE(pPropertyBag);
        SAFE_RELEASE(pVideo);
    }

    bRet = true;

SAFE_EXIT:
    errorMessage = errStream.str();
    SAFE_RELEASE(pVideoEnumerator);
    SAFE_RELEASE(pDevEnumCreater);

    return bRet;
}