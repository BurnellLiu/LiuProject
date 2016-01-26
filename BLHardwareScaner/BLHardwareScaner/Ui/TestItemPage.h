

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

/// @brief ������ҳ��
class TestItemPage : public QWidget
{
    Q_OBJECT

public:
    TestItemPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~TestItemPage();

protected:
    void showEvent(QShowEvent* e);

private:
    /// @brief ����һ��������
    /// @param[in] name ����������
    /// @param[in] icon ������ͼ��
    void AddTestItem(IN const QString& name, IN const QIcon& icon);

private:
    Ui::TestItemForm ui;
};

#endif