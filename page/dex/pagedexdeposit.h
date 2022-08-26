#ifndef PAGEDEXDEPOSIT_H
#define PAGEDEXDEPOSIT_H

#include <QWidget>
#include <QTimer>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageDexDeposit;
}

class PageDexDeposit : public QWidget
{
    Q_OBJECT

public:
    typedef struct {
        QString ticker;
        QString provider;
        QString contract;
        QString depositAddress;
        double minimalDeposit;
        double depositFee;
        QString confirmations;
    }entry_t;
    explicit PageDexDeposit(QWidget *parent = nullptr);
    ~PageDexDeposit();
    void open(void *arg1);
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_copiedFadeTimer();
    void on_backPushButton_clicked();
    void on_contractAddressPushButton_clicked();
    void on_depositAddressPushButton_clicked();

private:
    Ui::PageDexDeposit *ui;


    QTimer fadeTimer;
    int fadeCount = 0;

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect iconLabelQRectBack;
    QRect depositFundsForLabelQRectBack;
    QFont depositFundsForLabelQFontBack;
    QRect tickerLabelQRectBack;
    QFont tickerLabelQFontBack;
    QRect providerLabelQRectBack;
    QFont providerLabelQFontBack;
    QRect contractLabelQRectBack;
    QFont contractLabelQFontBack;
    QRect contractAddressPushButtonQRectBack;
    QFont contractAddressPushButtonQFontBack;
    QRect depositAddressLabelQRectBack;
    QFont depositAddressLabelQFontBack;
    QRect depositAddressPushButtonQRectBack;
    QFont depositAddressPushButtonQFontBack;
    QRect minimalDepositLabelQRectBack;
    QFont minimalDepositLabelQFontBack;
    QRect depositFeeLabelQRectBack;
    QFont depositFeeLabelQFontBack;
    QRect confirmationLabelQRectBack;
    QFont confirmationLabelQFontBack;

    QRect statusLabelQRectBack;
    QFont statusLabelQFontBack;
};

#endif // PAGEDEXDEPOSIT_H
