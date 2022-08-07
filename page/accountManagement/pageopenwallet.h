#ifndef PAGEOPENWALLET_H
#define PAGEOPENWALLET_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageOpenWallet;
}

class PageOpenWallet : public QWidget
{
    Q_OBJECT

public:
    explicit PageOpenWallet(QWidget *parent = nullptr);
    ~PageOpenWallet();
    void open();
    void setScale();
    void setStyle();
    void setVisibility(bool v);
    void retranslateUi();
    bool getVisibility();

private slots:
    void on_showHidePushButton_clicked();
    void on_openWalletPushButton_clicked();
    void on_newWalletPushButton_clicked();
    void on_importWalletPushButton_clicked();

    void on_walletNameLineEdit_textChanged(const QString &arg1);
    void on_walletPasswordLineEdit_textChanged(const QString &arg1);


private:
    Ui::PageOpenWallet *ui;
    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerLabelQRectBack;
    QFont headerLabelQFontBack;
    QRect walletNameLineEditQRectBack;
    QFont walletNameLineEditQFontBack;
    QRect walletPasswordLineEditQRectBack;
    QFont walletPasswordLineEditQFontBack;
    QRect showHidePushButtonQRectBack;
    QSize showHidePushButtonQSizeBack;
    QRect openWalletPushButtonQRectBack;
    QFont openWalletPushButtonQFontBack;
    QRect newWalletPushButtonQRectBack;
    QFont newWalletPushButtonQFontBack;
    QRect importWalletPushButtonQRectBack;
    QFont importWalletPushButtonQFontBack;
};

#endif // PAGEOPENWALLET_H
