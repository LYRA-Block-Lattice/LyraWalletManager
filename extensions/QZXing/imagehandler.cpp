#include "imagehandler.h"
#include <QGraphicsObject>
#include <QImage>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickWindow>
#include <QThread>
#include <QElapsedTimer>

ImageHandler::ImageHandler(QObject *parent) :
    QObject(parent)
{
}

QImage ImageHandler::extractQImage(QObject *imageObj, int offsetX, int offsetY, int width, int height)
{
#if QT_VERSION >= 0x050000
    QQuickItem *item = qobject_cast<QQuickItem *>(imageObj);

    if (!item || !item->window()->isVisible()) {
        qDebug() << "Item is NULL";
        return QImage();
    }

    QElapsedTimer timer;
    timer.start();
    QSharedPointer<QQuickItemGrabResult> result = item->grabToImage();
    pendingGrabbersLocker.lockForWrite();
    pendingGrabbers << result.data();
    pendingGrabbersLocker.unlock();

    connect(result.data(), &QQuickItemGrabResult::ready, this, &ImageHandler::imageGrabberReady);
    while (timer.elapsed() < 1000) {
        pendingGrabbersLocker.lockForRead();
        if (!pendingGrabbers.contains(result.data())) {
            pendingGrabbersLocker.unlock();
            break;
        }
        pendingGrabbersLocker.unlock();
        qApp->processEvents();
        QThread::yieldCurrentThread();
    }
    QImage img = result->image();
#else
    QGraphicsObject *item = qobject_cast<QGraphicsObject*>(imageObj);

    if (!item) {
        qDebug() << "Item is NULL";
        return QImage();
    }

    QImage img(item->boundingRect().size().toSize(), QImage::Format_RGB32);
    img.fill(QColor(255, 255, 255).rgb());
    QPainter painter(&img);
    QStyleOptionGraphicsItem styleOption;
    item->paint(&painter, &styleOption);
#endif

    if (offsetX < 0)
        offsetX = 0;
    if (offsetY < 0)
        offsetY = 0;
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;

    if (offsetX || offsetY || width || height)
        return img.copy(offsetX, offsetY, width, height);
    else
        return img;
}

void ImageHandler::save(QObject *imageObj, const QString &path,
                        const int offsetX, const int offsetY,
                        const int width, const int height)
{
    QImage img = extractQImage(imageObj, offsetX, offsetY, width, height);
    img.save(path);
}

#if QT_VERSION >= 0x050000
    void ImageHandler::imageGrabberReady()
    {
        pendingGrabbersLocker.lockForWrite();
        pendingGrabbers.remove(sender());
        pendingGrabbersLocker.unlock();
    }
#endif

