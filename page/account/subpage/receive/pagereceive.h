#ifndef PAGERECEIVE_H
#define PAGERECEIVE_H

#include <QWidget>
#include <QRect>
#include <QFont>
#include <QSize>
#include <QTimer>

#include "api/rpc/rpcclass.h"
#include "extensions/QZXing/QZXing.h"



namespace Ui {
class PageReceive;
}

class PageReceive : public QWidget
{
    Q_OBJECT

public:
    explicit PageReceive(QWidget *parent = nullptr);
    ~PageReceive();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_copiedFadeTimer();
    void on_backPushButton_clicked();
    void on_copyAddressPushButton_clicked();

private:
    Ui::PageReceive *ui;

    QTimer fadeTimer;
    int fadeCount = 0;

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect addressGroupBoxQRectBack;
    QFont addressGroupBoxQFontBack;
    QRect qrLabelQRectBack;
    QRect copyAddressHorizontalLayoutQRectBack;
    QFont addressLabelQFontBack;
    QRect copyAddressPushButtonQRectBack;
    QSize copyAddressPushButtonQSizeBack;

    QRect copiedLabelQRectBack;
    QFont copiedLabelQFontBack;
};

#endif // PAGERECEIVE_H
