
#include "TestItemPage.h"

#define STRING_TESTITEM_DISK_SPEED QObject::tr("Disk Speed")

TestItemPage::TestItemPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this); 

    // ��������
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
    // ��Ȼʹ��new����, ����pItem�����ø�����, ���Կ��Բ�delete
    QListWidgetItem* pItem = new QListWidgetItem(ui.listWidgetTestItem);
    pItem->setIcon(icon);
    pItem->setText(name);
    pItem->setTextAlignment(Qt::AlignHCenter);
    pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}