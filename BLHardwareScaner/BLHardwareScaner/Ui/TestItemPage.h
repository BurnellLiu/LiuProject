

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

/// @brief 测试项页面
class TestItemPage : public QWidget
{
    Q_OBJECT

public:
    TestItemPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~TestItemPage();

    /// @brief 设置UI调整比例
    void SetUIRatio(IN float ratio);



protected:
    /// @brief 窗口显示事件
    void showEvent(QShowEvent* e);

private:

    /// @brief 初始化测试项页面
    void Init();

    /// @brief 增加一个测试项
    /// @param[in] name 测试项名称
    /// @param[in] icon 测试项图标
    void AddTestItem(IN const QString& name, IN const QIcon& icon);

    /// @brief 加载UI样式
    void LoadQSS();

private slots:
    void TestItemClicked(QListWidgetItem* pItem);

private:
    Ui::TestItemForm ui;

    DiskSpeedPage* m_pDiskSpeedPage;

    float m_uiRatio; ///< UI需要调整的比例
    bool m_bInitDone; ///< 标记是否初始化完成
};

#endif