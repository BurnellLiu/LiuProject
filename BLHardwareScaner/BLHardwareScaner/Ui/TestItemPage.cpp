
#include "TestItemPage.h"

#define STRING_TESTITEM_DISK_SPEED QObject::tr("Disk Speed")

TestItemPage::TestItemPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this); 

    // 设置属性
    ui.listWidgetTestItem->setViewMode(QListView::IconMode);
    ui.listWidgetTestItem->setIconSize(QSize(96 , 96));
    ui.listWidgetTestItem->setMovement(QListView::Static);
    ui.listWidgetTestItem->setResizeMode(QListView::Adjust);

    this->AddTestItem(STRING_TESTITEM_DISK_SPEED, QIcon(".\\Image\\TestItem\\DiskSpeed.png"));
}

TestItemPage::~TestItemPage()
{

}

void TestItemPage::showEvent(QShowEvent* e)
{

}

void TestItemPage::AddTestItem(IN const QString& name, IN const QIcon& icon)
{
    // 虽然使用new方法, 但是pItem已设置父对象, 所以可以不delete
    QListWidgetItem* pItem = new QListWidgetItem(ui.listWidgetTestItem);
    pItem->setIcon(icon);
    pItem->setText(name);
    pItem->setTextAlignment(Qt::AlignHCenter);
    pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}