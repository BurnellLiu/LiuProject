

#ifndef _HARDWAREINFORPAGE_H_
#define _HARDWAREINFORPAGE_H_

#include <QtCore/QVector>

#include "ui_HardwareInfor.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

class HWItemInfor;

/// @brief Ӳ����Ϣҳ��
class HardwareInforPage : public QWidget
{
    Q_OBJECT

public:
    HardwareInforPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);
    ~HardwareInforPage();

protected:
    void showEvent(QShowEvent* e);

private slots:
    /// @brief ��ǰ��ѡ��ı�ۺ���
    void CurrentItemChanged();

private:
    /// @brief ��ʼ��Ӳ����Ϣ
    void InitHardwareInfor();

    /// @brief ����UI��ʽ
    void LoadQSS();

private:
    Ui::HardwareInforForm ui;

    QVector<HWItemInfor*> m_hwItemVec; ///< Ӳ�����б�
};

/// @brief Ӳ������Ϣ
class HWItemInfor
{
public:
    HWItemInfor();
    virtual ~HWItemInfor();

    /// @brief ��ȡ�����
    /// @return �����
    const QString& GetTitle() const;

    /// @brief ��ȡ������
    /// @return ������
    const QString& GetContent() const;


    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor() = 0;

protected:
    /// @brief ���������
    ///  
    /// @param[in] title ����
    void SetTitle(IN const QString& title);

    /// @brief ����������һ��
    /// @param[in] subTitle �����е��ӱ���
    /// @param[in] text �ı�
    void ContentAddItem(IN const QString& subTitle, IN const QString text);

    /// @brief ���������ӿ���
    void ContentAddBlankLine();

    /// @brief ������е���Ϣ
    void ClearInfor();

private:
    QString m_title; ///< �����
    QString m_content; ///< ������
};

/// @brief ���������Ϣ����Ϣ
class ComputerItemInfor : public HWItemInfor
{
public: 

    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief ����������Ϣ
class ProcessorItemInfor : public HWItemInfor
{
public:

    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief ��������Ϣ
class MotherBoardItemInfor : public HWItemInfor
{
public:

    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief �ڴ�����Ϣ
class MemoryItemInfor : public HWItemInfor
{
public:
     /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief ��������Ϣ
class DiskItemInfor : public HWItemInfor
{
public:
    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief �Կ�����Ϣ
class VideoCardItemInfor : public HWItemInfor
{
public:
    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief ��ʾ������Ϣ
class MonitorItemInfor : public HWItemInfor
{
public:
    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief �������Ϣ
class BatteryItemInfor : public HWItemInfor
{
public:
    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

/// @brief ��������Ϣ
class NetworkCardItemInfor : public HWItemInfor
{
public:
    /// @brief ����Ӳ����Ϣ
    virtual void LoadHWInfor();
};

#endif