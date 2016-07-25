

#ifndef _FTP_DOWNLOAD_
#define _FTP_DOWNLOAD_

#include <QtNetwork/QFtp>
#include <QtCore/QFile>



#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif


/// @brief �����ļ�״̬
struct DownloadState
{
    bool IsDone; ///< ��־�����Ƿ����
    qint64 DoneSize; ///< ��ɴ�С
    qint64 TotalSize; ///< �ļ��ܴ�С

    bool IsError; ///< ��־�Ƿ�������
    QString ErrorMsg; ///< ������Ϣ
};



/// @brief FTP������
class LFtpDownload : public QObject
{
    Q_OBJECT

public:
    /// @brief ���캯��
    /// @param[in] url �����ļ�FTP URL
    explicit LFtpDownload(IN const QString& url);

    /// @brief ��������
    /// @return
    ~LFtpDownload();

    /// @brief ���õ�½��Ϣ
    /// @param[in] user �û���
    /// @param[in] pwd ����
    void SetLogin(IN const QString& user, IN const QString& pwd);

    /// @brief ���ñ����ļ���·��
    /// @param[in] path ·��
    /// @return
    void SetFilePath(IN const QString& path);

    /// @brief ��ʼ����
    /// �÷���Ϊ�첽����
    void StartDownloadAsync();

    /// @brief ��ȡ����״̬
    /// @return ����״̬
    const DownloadState& GetDownloadState();

private slots:
    /// @brief ������ɲۺ���
    /// @param[in] bError ��־�Ƿ�������
    void FtpDownloadDone(bool bError);

    /// @brief ���ݴ������
    /// @param[in] done ������ֽ���
    /// @param[in] total ���ֽ���
    void DataTransferProgress(qint64 done, qint64 total);

private:
    QFtp m_ftp; ///< FTP����
    QFile m_downloadFile; ///< ������ļ�����
    QString m_filePath; ///< �ļ�����·��
    QString m_downloadUrl; ///< ��������

    QString m_loginUser; ///< FTP��¼�û���
    QString m_loginPwd; ///< FTP��¼����

    DownloadState m_downloadState; ///< �洢����״̬
};

#endif