#ifndef PAGEDEXWITHDRAW_H
#define PAGEDEXWITHDRAW_H

#include <QWidget>
#include <QTimer>

#include <QRect>
#include <QFont>
#include <QSize>

#include "page/account/subpage/send/camera.h"

namespace Ui {
class PageDexWithdraw;
}

class PageDexWithdraw : public QWidget
{
    Q_OBJECT

public:
    typedef struct {
        QString ticker;
        QString provider;
        QString contract;
        double withdrawFee;
        double amountInAccount;
    }entry_t;
    explicit PageDexWithdraw(QWidget *parent = nullptr);
    ~PageDexWithdraw();
    void open(void *arg1);
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_copiedFadeTimer();
    void on_recipientAddressLineEdit_textChanged(const QString &arg1);
    void on_qrPushButton_clicked();
    void on_amountLineEdit_textChanged(const QString &arg1);
    void on_maximumPushButton_clicked();
    void on_withdrawPushButton_clicked();
    void on_backPushButton_clicked();
    void on_contractAddressPushButton_clicked();

private:
    Ui::PageDexWithdraw *ui;
    Camera *camera = nullptr;

    entry_t Entry;

    QTimer fadeTimer;
    int fadeCount = 0;

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect iconLabelQRectBack;
    QRect withdrawFundsForLabelQRectBack;
    QFont withdrawFundsForLabelQFontBack;
    QRect tickerLabelQRectBack;
    QFont tickerLabelQFontBack;
    QRect providerLabelQRectBack;
    QFont providerLabelQFontBack;
    QRect contractLabelQRectBack;
    QFont contractLabelQFontBack;
    QRect contractAddressPushButtonQRectBack;
    QFont contractAddressPushButtonQFontBack;
    QRect withdrawFeeLabelQRectBack;
    QFont withdrawFeeLabelQFontBack;
    QRect addressToLabelQRectBack;
    QFont addressToLabelQFontBack;
    QRect recipientAddressLineEditQRectBack;
    QFont recipientAddressLineEditQFontBack;
    QRect qrPushButtonQRectBack;
    QSize qrPushButtonQSizeBack;
    QRect amountLineEditQRectBack;
    QFont amountLineEditQFontBack;
    QRect maximumPushButtonQRectBack;
    QSize maximumPushButtonQSizeBack;
    QRect withdrawPushButtonQRectBack;
    QFont withdrawPushButtonQFontBack;

    QRect statusLabelQRectBack;
    QFont statusLabelQFontBack;

    bool checkCameraAvailability();
};

#endif // PAGEDEXWITHDRAW_H
