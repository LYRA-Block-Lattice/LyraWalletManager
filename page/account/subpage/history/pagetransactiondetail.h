#ifndef PAGETRANSACTIONDETAIL_H
#define PAGETRANSACTIONDETAIL_H

#include <QWidget>
#include <QRect>
#include <QFont>
#include <QSize>
#include <QTimer>

#include "api/rpc/rpcclass.h"

namespace Ui {
class PageTransactionDetail;
}

class PageTransactionDetail : public QWidget
{
    Q_OBJECT

public:
    explicit PageTransactionDetail(QWidget *parent = nullptr);
    ~PageTransactionDetail();
    void open(RpcClass::History::entry_t *entry);
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_copiedFadeTimer();
    void on_backPushButton_clicked();
    void on_copyTimeStampPushButton_clicked();
    void on_openSendAccountIdPushButton_clicked();
    void on_openSendHashPushButton_clicked();
    void on_openReceiveAccountIdPushButton_clicked();
    void on_openReceiveHashPushButton_clicked();
    void on_copyAmountTransferedPushButton_clicked();
    void on_copyAmountInAccountPushButton_clicked();

private:
    Ui::PageTransactionDetail *ui;
    QTimer fadeTimer;
    int fadeCount = 0;

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect lyraLogoLabelQRectBack;
    //QSize lyraLogoLabelQSizeBack;
    QRect label1QRectBack;
    QFont label1QFontBack;

    QRect heightLabelQRectBack;
    QFont heightLabelQFontBack;
    QRect directionLabelQRectBack;
    QFont directionLabelQFontBack;
    QRect timeStampLabelQRectBack;
    QFont timeStampLabelQFontBack;
    QRect sendAccountIdLabelQRectBack;
    QFont sendAccountIdLabelQFontBack;
    QRect sendHashLabelQRectBack;
    QFont sendHashLabelQFontBack;
    QRect receiveAccountIdLabelQRectBack;
    QFont receiveAccountIdLabelQFontBack;
    QRect receiveHashLabelQRectBack;
    QFont receiveHashLabelQFontBack;
    QRect amountTransferredLabelQRectBack;
    QFont amountTransferredLabelQFontBack;
    QRect amountInAccountLabelQRectBack;
    QFont amountInAccountLabelQFontBack;

    QRect heightValueLabelQRectBack;
    QFont heightValueLabelQFontBack;
    QRect directionValueLabelQRectBack;
    QFont directionValueLabelQFontBack;
    QRect timeStampValueLabelQRectBack;
    QFont timeStampValueLabelQFontBack;
    QRect sendAccountIdValueLabelQRectBack;
    QFont sendAccountIdValueLabelQFontBack;
    QRect sendHashValueLabelQRectBack;
    QFont sendHashValueLabelQFontBack;
    QRect receiveAccountIdValueLabelQRectBack;
    QFont receiveAccountIdValueLabelQFontBack;
    QRect receiveHashValueLabelQRectBack;
    QFont receiveHashValueLabelQFontBack;
    QRect amountTransferredValueLabelQRectBack;
    QFont amountTransferredValueLabelQFontBack;
    QRect amountInAccountValueLabelQRectBack;
    QFont amountInAccountValueLabelQFontBack;

    QRect directionPushButtonQRectBack;
    QSize directionPushButtonIconQSizeBack;
    QRect copyTimeStampPushButtonQRectBack;
    QSize copyTimeStampPushButtonIconQSizeBack;
    QRect openSendAccountIdPushButtonQRectBack;
    QSize openSendAccountIdPushButtonIconQSizeBack;
    QRect openSendHashPushButtonQRectBack;
    QSize openSendHashPushButtonIconQSizeBack;
    QRect openReceiveAccountIdPushButtonQRectBack;
    QSize openReceiveAccountIdPushButtonIconQSizeBack;
    QRect openReceiveHashPushButtonQRectBack;
    QSize openReceiveHashPushButtonIconQSizeBack;
    QRect copyAmountTransferedPushButtonQRectBack;
    QSize copyAmountTransferedPushButtonIconQSizeBack;
    QRect copyAmountInAccountPushButtonQRectBack;
    QSize copyAmountInAccountPushButtonIconQSizeBack;

    QRect line_1LineQRectBack;
    QRect line_2LineQRectBack;
    QRect line_3LineQRectBack;
    QRect line_4LineQRectBack;
    QRect line_5LineQRectBack;
    QRect line_6LineQRectBack;
    QRect line_7LineQRectBack;

    QRect copiedLabelQRectBack;
    QFont copiedLabelQFontBack;

    RpcClass::History::entry_t *entry;
};

#endif // PAGETRANSACTIONDETAIL_H
