#include "camera.h"
//#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
//#include "ui_camera_mobile.h"
//#else
#include "ui_camera.h"
//#endif
//#include "videosettings.h"
//#include "imagesettings.h"
//#include "metadatadialog.h"

#include <QMediaRecorder>
#include <QVideoWidget>
#include <QCameraDevice>
#include <QMediaMetaData>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioInput>

#include <QMessageBox>
#include <QPalette>
#include <QImage>
#include <QPixmap>

#include <QtWidgets>
#include <QMediaDevices>
#include <QMediaFormat>

Camera::Camera(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Camera) {
    ui->setupUi(this);
    setParent(parent);
    this->setWindowFlags(( windowFlags() & ~(Qt::WindowFullscreenButtonHint | Qt::WindowMaximizeButtonHint)) );
    qrDecoder = new QZXing(QZXing::DecoderFormat::DecoderFormat_QR_CODE);
    qrDecoder->setTryHarder(true);
    //Camera devices:
    videoDevicesGroup = new QActionGroup(this);
    videoDevicesGroup->setExclusive(true);
    updateCameras();
    connect(&m_devices, &QMediaDevices::videoInputsChanged, this, &Camera::updateCameras);

    connect(videoDevicesGroup, &QActionGroup::triggered, this, &Camera::updateCameraDevice);

    setCamera(QMediaDevices::defaultVideoInput());

    loopTimer.setInterval(250);
    connect(&loopTimer, &QTimer::timeout, this, &Camera::on_timerLoopTick);
    loopTimer.start();
}

void Camera::setCamera(const QCameraDevice &cameraDevice) {
    m_camera.reset(new QCamera(cameraDevice));
    m_captureSession.setCamera(m_camera.data());

    connect(m_camera.data(), &QCamera::activeChanged, this, &Camera::updateCameraActive);
    connect(m_camera.data(), &QCamera::errorOccurred, this, &Camera::displayCameraError);

    m_imageCapture = new QImageCapture;
    m_captureSession.setImageCapture(m_imageCapture);

    m_captureSession.setVideoOutput(ui->viewfinder);

    updateCameraActive(m_camera->isActive());

    connect(m_imageCapture, &QImageCapture::readyForCaptureChanged, this, &Camera::readyForCapture);
    connect(m_imageCapture, &QImageCapture::imageCaptured, this, &Camera::processCapturedImage);
    connect(m_imageCapture, &QImageCapture::imageSaved, this, &Camera::imageSaved);
    connect(m_imageCapture, &QImageCapture::errorOccurred, this, &Camera::displayCaptureError);
    readyForCapture(m_imageCapture->isReadyForCapture());

    /*if (m_camera->cameraFormat().isNull()) {
        auto formats = cameraDevice.videoFormats();
        if (!formats.isEmpty()) {
            // Choose a decent camera format: Maximum resolution at at least 30 FPS
            // we use 29 FPS to compare against as some cameras report 29.97 FPS...
            QCameraFormat bestFormat;
            for (const auto &fmt : formats) {
                if (bestFormat.maxFrameRate() < 29 && fmt.maxFrameRate() > bestFormat.maxFrameRate())
                    bestFormat = fmt;
                else if (bestFormat.maxFrameRate() == fmt.maxFrameRate() &&
                         bestFormat.resolution().width()*bestFormat.resolution().height() <
                             fmt.resolution().width()*fmt.resolution().height())
                    bestFormat = fmt;
            }

            m_camera->setCameraFormat(bestFormat);
        }
    }*/

    m_camera->start();
}

/*void Camera::keyReleaseEvent(QKeyEvent *event) {
    QMainWindow::keyReleaseEvent(event);
}*/

void Camera::processCapturedImage(int requestId, const QImage& img) {
    Q_UNUSED(requestId);
    QImage scaledImage = img.scaled(ui->viewfinder->size(),
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);

    //ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(scaledImage));
    m_isCapturingImage = false;
    QString qrCode = qrDecoder->decodeImage(QPixmap::fromImage(scaledImage).toImage());
    if(qrCode.length() > 10) {
        emit qrRead(qrCode);
        this->close();
        this->deleteLater();
    }
}

void Camera::takeImage() {
    m_isCapturingImage = true;
    m_imageCapture->capture();
}

void Camera::displayCaptureError(int id, const QImageCapture::Error error, const QString &errorString) {
    Q_UNUSED(id);
    Q_UNUSED(error);
    //Q_UNUSED(errorString);
    m_isCapturingImage = false;
    qDebug() << errorString;
    //emit qrRead("");
    //this->close();
}

/*void Camera::startCamera() {
    m_camera->start();
}

void Camera::stopCamera() {
    m_camera->stop();
}*/

void Camera::updateCameraActive(bool active) {
    if (active) {
        ui->actionStartCamera->setEnabled(false);
        ui->actionStopCamera->setEnabled(true);
        //ui->captureWidget->setEnabled(true);
        ui->actionSettings->setEnabled(true);
    } else {
        ui->actionStartCamera->setEnabled(true);
        ui->actionStopCamera->setEnabled(false);
        //ui->captureWidget->setEnabled(false);
        ui->actionSettings->setEnabled(false);
    }
}

void Camera::displayCameraError() {
    if (m_camera->error() != QCamera::NoError)
        QMessageBox::warning(this, tr("Camera Error"), m_camera->errorString());
}

void Camera::updateCameraDevice(QAction *action) {
    setCamera(qvariant_cast<QCameraDevice>(action->data()));
}

void Camera::readyForCapture(bool ready) {
    m_readyForCapture = ready;
    //ui->takeImageButton->setEnabled(ready);
}

void Camera::imageSaved(int id, const QString &fileName) {
    Q_UNUSED(id);

    m_isCapturingImage = false;
    if (m_applicationExiting)
        close();
}

void Camera::closeEvent(QCloseEvent *event) {
    loopTimer.stop();
    setEnabled(false);
    m_applicationExiting = true;
    emit windowClosed();
    event->accept();
    this->deleteLater();
}

void Camera::updateCameras() {
    ui->menuDevices->clear();
    const QList<QCameraDevice> availableCameras = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : availableCameras) {
        QAction *videoDeviceAction = new QAction(cameraDevice.description(), videoDevicesGroup);
        videoDeviceAction->setCheckable(true);
        videoDeviceAction->setData(QVariant::fromValue(cameraDevice));
        if (cameraDevice == QMediaDevices::defaultVideoInput())
            videoDeviceAction->setChecked(true);

        ui->menuDevices->addAction(videoDeviceAction);
    }
}

void Camera::on_timerLoopTick() {
    if(!m_isCapturingImage && m_readyForCapture)
        takeImage();
}

void Camera::on_backPushButton_clicked() {
    this->close();
    this->deleteLater();
}

