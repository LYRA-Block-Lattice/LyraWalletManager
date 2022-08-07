#ifndef PAGESEND_H
#define PAGESEND_H

#include <QWidget>
#include <QRect>
#include <QFont>
#include <QSize>
#include <QTimer>

#include "api/rpc/rpcclass.h"
#include "wallet/walletrpc.h"

#include "page/account/subpage/send/camera.h"


namespace Ui {
class PageSend;
}

class PageSend : public QWidget
{
    Q_OBJECT

public:
    explicit PageSend(QWidget *parent = nullptr);
    ~PageSend();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_sendingFadeTimer();
    void on_backPushButton_clicked();
    void on_recipientAddressLineEdit_textChanged(const QString &arg1);
    void on_amountLineEdit_textChanged(const QString &arg1);
    void on_qrPushButton_clicked();
    void on_qrCodeRead(QString qr);
    void on_qrWindowClosed();
    void on_sendPushButton_clicked();

    void on_SendRetriveDone(const QString &s);
    void on_SendRetriveError(const QString &s);

private:
    Ui::PageSend *ui;

    void checkIntegrityOfInputs();

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect tokenSelectComboBoxQRectBack;
    QFont tokenSelectComboBoxQFontBack;
    QRect recipientAddressLineEditQRectBack;
    QFont recipientAddressLineEditQFontBack;
    QRect amountLineEditQRectBack;
    QFont amountLineEditQFontBack;

    QRect qrPushButtonQRectBack;
    QSize qrPushButtonQSizeBack;
    QRect pastePushButtonQRectBack;
    QSize pastePushButtonQSizeBack;

    QRect sendPushButtonQRectBack;
    QFont sendPushButtonQFontBack;

    QRect sendingLabelQRectBack;
    QFont sendingLabelQFontBack;

    Camera *camera = nullptr;

    WalletRpc::Send *sendThread = nullptr;
    QThread *sendWorkerThread = nullptr;
signals:
    void sendOperate(QString amount, QString destAccount, QString ticker);
};

#endif // PAGESEND_H
