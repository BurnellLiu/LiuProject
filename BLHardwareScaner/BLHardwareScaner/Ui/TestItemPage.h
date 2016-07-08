

#ifndef _TESTITEMPAGE_H_
#define _TESTITEMPAGE_H_

#include "ui_TestItem.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class DiskSpeedPage;
class CameraViewPage;

/// @brief ������ҳ��
class TestItemPage : public QWidget
{
    Q_OBJECT

public:
    /// @brief ���캯��
    /// @param[in] uiRatio UI�Ŵ�ϵ��
    TestItemPage(IN float uiRatio, IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~TestItemPage();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private:

    /// @brief ��ʼ��������ҳ��
    void Init();

    /// @brief ����һ��������
    /// @param[in] name ����������
    /// @param[in] icon ������ͼ��
    void AddTestItem(IN const QString& name, IN const QIcon& icon);

    /// @brief ����UI��ʽ
    void LoadQSS();

private slots:
    void TestItemClicked(QListWidgetItem* pItem);

private:
    Ui::TestItemForm ui;

    DiskSpeedPage* m_pDiskSpeedPage;
    CameraViewPage* m_pCameraViewPage;

    bool m_bCameraExist; ///< ����Ƿ��������ͷ

    float m_uiRatio; ///< UI��Ҫ�����ı���
    bool m_bInitDone; ///< ����Ƿ��ʼ�����
};

#endif