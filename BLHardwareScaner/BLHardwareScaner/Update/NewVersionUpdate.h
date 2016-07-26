

#ifndef _NEWVERSION_UPDATE_H_
#define _NEWVERSION_UPDATE_H_

#include "FtpDownload.h"

/// @brief ����°汾���
enum CHECKNEW_RESULT
{
    CHECKNEW_UNKNOWN = 0, ///< δ֪
    CHECKNEW_RUNNING, ///< ��������
    CHECKNEW_WITH_ERROR, ///< �д���
    CHECKNEW_WITH_NEW, ///< ���µĸ���
    CHECKNEW_WITHOUT_NEW ///< û���µĸ���
};

/// @brief ����°汾
class CheckNew
{
public:
    /// @brief ���캯��
    CheckNew();

    /// @brief ��������
    ~CheckNew();

    /// @brief ���õ�ǰ�汾, ��ʽ: V1.1.1
    /// @param[in] version ��ǰ�汾���ڱȽ�
    void SetCurrentVersion(const QString& version);

    /// @brief ��ʼ����°汾
    /// �÷���Ϊ�첽����
    /// �÷�������������UI�߳���
    void StartCheckAsync();

    /// @brief ��ȡ���
    /// @return ����°汾���
    CHECKNEW_RESULT GetResult();

private:
    QString m_currentVersion; ///< ��ǰ�汾, ��ʽ: V1.1.1
    LFtpDownload* m_pFtpDownload; ///< FTP���ض���
};


// ���ش���
#define DOWNLOADNEW_ERROR -1

// �������
#define DOWNLOADNEW_DONE 101

/// @brief �����°汾
class DownloadNew
{
public:
    /// @brief ���캯��
    DownloadNew();

    /// @brief ��������
    ~DownloadNew();

    /// @brief ��ʼ�����°汾
    /// �÷���Ϊ�첽����
    /// �÷�������������UI�߳���
    void StartDownloadAsync();

    /// @brief ��ȡ���
    /// ����ֵ[0, 100]��ʾ�������ص����ؽ���, ����ֵΪ-1���ʾ��������, ����ֵΪ101���ʾ�������
    /// @return �����°汾���
    int GetResult();

private:
    LFtpDownload* m_pFtpDownload; ///< FTP���ض���
};

#endif