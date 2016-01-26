
#include "TestItemPage.h"

#include "TestItem\\DiskSpeedPage.h"

#define STRING_TESTITEM_DISK_SPEED QObject::tr("Disk Speed")

TestItemPage::TestItemPage(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent, flags)
{
    ui.setupUi(this); 

    m_bInitDone = false;
    m_uiRatio = 1.0f;
}

TestItemPage::~TestItemPage()
{

}

void TestItemPage::SetUIRatio(IN float ratio)
{
    m_uiRatio = ratio;
}


void TestItemPage::showEvent(QShowEvent* e)
{
    if (!m_bInitDone)
    {
        this->Init();
        m_bInitDone = true;
    }
}

void TestItemPage::Init()
{
    connect(ui.listWidgetTestItem, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(TestItemDoubleClicked(QListWidgetItem*)));
    // 设置属性
    ui.listWidgetTestItem->setViewMode(QListView::IconMode);
    ui.listWidgetTestItem->setIconSize(QSize(96 * m_uiRatio, 96 * m_uiRatio));
    ui.listWidgetTestItem->setMovement(QListView::Static);
    ui.listWidgetTestItem->setResizeMode(QListView::Adjust);

    this->AddTestItem(STRING_TESTITEM_DISK_SPEED, QIcon(".\\Image\\TestItem\\DiskSpeed.png"));
}

void TestItemPage::AddTestItem(IN const QString& name, IN const QIcon& icon)
{
    // 虽然使用new方法, 但是pItem已设置父对象, 所以可以不delete
    // Qt机制: 父对象被析构时, 自动析构子对象
    QListWidgetItem* pItem = new QListWidgetItem(ui.listWidgetTestItem);
    pItem->setIcon(icon);
    pItem->setText(name);
    pItem->setTextAlignment(Qt::AlignHCenter);
    pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void TestItemPage::DiskSpeedClick()
{
    DiskSpeedPage page(this);
    page.exec();
}

void TestItemPage::TestItemDoubleClicked(QListWidgetItem* pItem)
{
    if (NULL == pItem)
        return;

    if (pItem->text() == STRING_TESTITEM_DISK_SPEED)
    {
        this->DiskSpeedClick();
    }
}