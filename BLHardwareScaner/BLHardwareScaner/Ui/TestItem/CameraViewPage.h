

#ifndef _CAMERAVIRW_PAGE_H_
#define _CAMERAVIEW_PAGE_H_

#include <QtGui/QDialog>
#include <QtCore/QVector>

#include "ui_CameraView.h"

#include "..\\Src\\LHardwareInfor.h"
#include "..\\Src\\Webcam\\LWebcam.h"

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef OUT
#define OUT
#endif

/// @brief ����ͷԤ��ҳ��
class CameraViewPage : public QDialog
{
    Q_OBJECT

public:
    /// @brief ���캯��
    CameraViewPage(IN QWidget *parent = 0, IN Qt::WFlags flags = 0);

    /// @brief ��������
    ~CameraViewPage();

protected:
    /// @brief ������ʾ�¼�
    void showEvent(QShowEvent* e);

    /// @brief ���������¼�
    void hideEvent(QHideEvent* e);

private slots:
   /// @brief ת����ť�����ۺ���
   void SwitchButtonClicked();

private:
   /// @brief ����UI��ʽ
   void LoadQSS();

private:
    /// @brief ��ʾ��ǰ����ͷ
    void ShowCurrentCamera();

    /// @brief �رյ�ǰ����ͷ
    void CloseCurrentCamera();

    /// @brief ��ȡWebcam�ӿ�ָ��
    /// @param[in] index Webcam����
    LWebcam* GetWebcam(IN unsigned int index);


private:
    Ui::CameraViewForm ui;

    int m_currentCameraIndex; ///< ��ǰ��ʾ������ͷ����
    unsigned int m_cameraTotalCount; ///< ����ͷ����
    LWebcam* m_pCurrentWebcam; ///< ��ǰ��ʾ������ͷ
};

#endif