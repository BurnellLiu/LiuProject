

#ifndef _LWEBCAM_H_
#define _LWEBCAM_H_

#include <string>
using std::string;
using std::wstring;

#include <vector>
using std::vector;



#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


class CWebcam;


/// <SUMMARY>
/// λͼ�ṹ
/// </SUMMARY>
class CDBIImage
{
	friend class CWebcam;

public:
	CDBIImage();
	~CDBIImage();

	/// <SUMMARY>
	/// ��ȡ����ͼ��ƽ������ֵ
	/// </SUMMARY>
	/// <PARAM name = "avgLuma" dir = "OUT">
	/// �洢����ֵ
	/// </PARAM>
	/// <RETURNS>
	/// �ɹ�����true, ʧ�ܷ���false
	/// </RETURNS>
	bool GetAvgLuma(OUT int& avgLuma);

	/// <SUMMARY>
	/// ����ΪBMP�ļ�
	/// </SUMMARY>
	/// <PARAM name = "fileName" dir = "IN">
	/// �ļ���
	/// </PARAM>
	/// <RETURNS>
	/// �ɹ�����true, ʧ�ܷ���false
	/// </RETURNS>
	bool SaveToBMP(IN const char* fileName);

private:
	long Size; // λͼ��С
	unsigned char* Data; // λͼ����

private:
	CDBIImage(const CDBIImage&);
	CDBIImage& operator = (const CDBIImage&);
};

/// <SUMMARY>
/// ����ͷ�ӿ�
/// </SUMMARY>
class LWebcam
{
public:
    /// <SUMMARY>
    /// ��������
    /// </SUMMARY>
    virtual ~LWebcam() = 0 {}

    /// <SUMMARY>
    /// ���ø�����
    /// </SUMMARY>
    /// <PARAM name = "hwnd" dir = "IN">
    /// �����ھ��
    /// </PARAM>
    /// <PARAM name = "left, top, width, height" dir = "IN">
    /// ����ͷ�����ڸ������е�λ�úʹ�С, ����ͷ���������, ����ͷ���������, ����ͷ���ڿ��, ����ͷ���ڸ߶�
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool SetParentWindow(IN void* hwnd, IN long left, IN long top, IN long width, IN long height) = 0;

    /// <SUMMARY>
    /// ��ȡ����ͷ�Ѻ�����
    /// </SUMMARY>
    /// <PARAM name = "name" dir = "OUT">
    /// �洢����ͷ�Ѻ�����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool GetFriendlyName(OUT wstring& name) = 0;

    /// <SUMMARY>
    /// ��ȡ����ͷ��ʾ����
    /// </SUMMARY>
    /// <PARAM name = "name" dir = "OUT">
    /// �洢����ͷ��ʾ����
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool GetDisplayName(OUT wstring& name) = 0;

    /// <SUMMARY>
    /// ��ȡ��ǰͼ��
    /// </SUMMARY>
    /// <PARAM name = "image" dir = "OUT">
    /// �洢ͼ����Ϣ
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool GetCurrentImage(OUT CDBIImage& image) = 0;

    /// <SUMMARY>
    /// ��������ͷ
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool Run() = 0;

    /// <SUMMARY>
    /// ��ͣ����ͷ
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool Pause() = 0;

    /// <SUMMARY>
    /// ֹͣ����ͷ
    /// </SUMMARY>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool Stop() = 0;

    /// <SUMMARY>
    /// ��ȡƽ��֡�ٶ�(ÿ��)
    /// </SUMMARY>
    /// <PARAM name = "rate" dir = "IN">
    /// �洢֡�ٶ�
    /// </PARAM>
    /// <RETURNS>
    /// �ɹ�����true, ʧ�ܷ���false, ʹ��GetErrorMessage()������ȡ������Ϣ
    /// </RETURNS>
    virtual bool GetAvgFrameRate(int& rate) = 0;

    /// <SUMMARY>
    /// ��ȡ������Ϣ
    /// </SUMMARY>
    /// <RETURNS>
    /// ������Ϣ
    /// </RETURNS>
    virtual  string GetErrorMessage() = 0;

    /// <SUMMARY>
    /// ��ȡ������Ϣ
    /// </SUMMARY>
    virtual void GetErrorMessage(string& error) = 0;


    /// <SUMMARY>
    /// �ͷ���Դ
    /// </SUMMARY>
    virtual void Release() = 0;
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
bool LEnumWebcam(OUT vector<LWebcam*>& webcamList, string& errorMessage);


#endif