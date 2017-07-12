

#ifndef _GPUSTRESS_PAGE_H_
#define _GPUSTRESS_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QVector>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtCore/QTimer>

#include "ui_GPUStress.h"


#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief GPUѹ������ҳ��
class GPUStressPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief ���캯��
    /// @param[in] uiRatio UI�Ŵ�ϵ��
    GPUStressPage(IN float uiRatio, IN QWidget *parent = 0, IN Qt::WFlags flags = 0);

    /// @brief ��������
    ~GPUStressPage();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

private slots:
    /// @brief ���԰�ť�����ۺ���
    void TestButtonClicked();

    /// @brief GPUѹ�����Զ�ʱ����ʱ����
    void GpuStressTimerTimeout();

private:
    Ui::GPUStressForm ui;
    QProcess m_gpuProcess; ///< GPUѹ�����Խ���
    QTimer m_gpuTimer; ///< GPUѹ�����Խ��̼�ʱ��
};

#endif