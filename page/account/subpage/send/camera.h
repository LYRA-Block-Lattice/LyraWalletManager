#ifndef CAMERA_H_
#define CAMERA_H_

#include <QWidget>
#include <QCamera>
#include <QImageCapture>
#include <QMediaRecorder>
#include <QScopedPointer>
#include <QMediaMetaData>
#include <QMediaCaptureSession>
#include <QMediaDevices>
#include <QAudioInput>
#include <QTimer>

#include <QMainWindow>

#include "extensions/QZXing/QZXing.h"

namespace Ui {
    class Camera;
}
QT_BEGIN_NAMESPACE
class QActionGroup;
QT_END_NAMESPACE

class MetaDataDialog;

class Camera : public QMainWindow {
    Q_OBJECT

public:
    Camera(QWidget *parent = nullptr);

private slots:
    void setCamera(const QCameraDevice &cameraDevice);

    //void startCamera();
    //void stopCamera();

    void displayCaptureError(int, QImageCapture::Error, const QString &errorString);

    void displayCameraError();

    void updateCameraDevice(QAction *action);

    void updateCameraActive(bool active);

    void processCapturedImage(int requestId, const QImage &img);

    void readyForCapture(bool ready);
    void imageSaved(int id, const QString &fileName);

    void updateCameras();
    void on_timerLoopTick();

    void on_backPushButton_clicked();

protected:
    //void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void takeImage();

    Ui::Camera *ui;

    QActionGroup *videoDevicesGroup  = nullptr;

    QMediaDevices m_devices;
    QMediaCaptureSession m_captureSession;
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QAudioInput> m_audioInput;
    QImageCapture *m_imageCapture;

    bool m_isCapturingImage = false;
    bool m_applicationExiting = false;
    bool m_doImageCapture = true;
    bool m_readyForCapture = false;

    QZXing *qrDecoder = nullptr;

    QTimer loopTimer;
signals:
    void qrRead(QString qr);
    void windowClosed();
};
#endif
