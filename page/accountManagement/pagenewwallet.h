#ifndef PAGENEWWALLET_H
#define PAGENEWWALLET_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageNewWallet;
}

class PageNewWallet : public QWidget
{
    Q_OBJECT

public:
    explicit PageNewWallet(QWidget *parent = nullptr);
    ~PageNewWallet();
    void open();
    void setScale();
    void setStyle();
    void setVisibility(bool v);
    bool getVisibility();

private slots:
    void on_showHidePushButton_clicked();
    void on_backPushButton_clicked();
    void on_walletNameLineEdit_textChanged(const QString &arg1);
    void on_walletPasswordLineEdit_textChanged(const QString &arg1);
    void on_walletPasswordLineEdit_2_textChanged(const QString &arg1);
    void on_createWalletPushButton_clicked();

private:
    Ui::PageNewWallet *ui;
    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerLabelQRectBack;
    QFont headerLabelQFontBack;
    QRect walletNameLineEditQRectBack;
    QFont walletNameLineEditQFontBack;
    QRect walletPasswordLineEditQRectBack;
    QFont walletPasswordLineEditQFontBack;
    QRect walletPasswordLineEdit2QRectBack;
    QFont walletPasswordLineEdit2QFontBack;
    QRect showHidePushButtonQRectBack;
    QSize showHidePushButtonQSizeBack;
    QRect createWalletPushButtonQRectBack;
    QFont createWalletPushButtonQFontBack;
};

#endif // PAGENEWWALLET_H
