

#ifndef _DISKSPEED_PAGE_H_
#define _DISKSPEED_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QVector>
#include <QtCore/QTimer>

#include "ui_DiskSpeed.h"
#include "..\\Src\\LDiskSpeed.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief �����ٶȲ���ҳ��
class DiskSpeedPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief ���캯��
    DiskSpeedPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);

    /// @brief ��������
    ~DiskSpeedPage();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private slots:
    /// @brief ���԰�ť�����ۺ���
    void TestButtonClicked();

    /// @brief ˳����Լ���ʱ����ʱ�ص�����
    void SeqTestMonitorTimer();

    /// @brief 4K������Լ���ʱ����ʱ�ص�����
    void Rand4KTestMonitorTimer();

private:
    /// @brief ���´�����Ϣ
    void UpdateDiskInformation();

    /// @brief �Ӵ��̵��߼�������ѡ��һ��
    /// ����ѡ��ʣ��ռ��ķ���
    /// @param[in] diskLogicalName �����߼�����, ��������ԷֺŸ�����: C:;D:;E:
    /// @param[out] pDrive �洢ѡ����߼�����
    /// @param[out] pDriveFreeSpace �߼�������ʣ��ռ�, ��λ�ֽ�
    /// @return �ɹ�����true, ʧ�ܷ���false
    bool SelectLogicalDrive(
        IN const QString& diskLogicalName, 
        OUT QString* pDrive, 
        OUT unsigned long long* pDriveFreeSpace);

private:
    Ui::DiskSpeedForm ui;

    QTimer m_seqTestTimer; ///< ˳����Լ�ⶨʱ��
    QTimer m_rand4KTestTimer; ///< 4K������Լ�ⶨʱ��

    IDiskSpeedTest* m_pSeqTest; ///< ˳�����
    IDiskSpeedTest* m_p4KRandTest; ///< 4K�������

    QString m_currentTestLogicalDrive; ///< ��ǰ��Ҫ���Ե��߼�����
    QVector<QString> m_diskLogicalNameList; ///< �����߼��̷������б�

};

#endif