#ifndef PAGEIMPORTWALLET_H
#define PAGEIMPORTWALLET_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageImportWallet;
}

class PageImportWallet : public QWidget
{
    Q_OBJECT

public:
    explicit PageImportWallet(QWidget *parent = nullptr);
    ~PageImportWallet();
    void open();
    void setScale();
    void setStyle();
    void setVisibility(bool v);
    bool getVisibility();

private slots:
    void on_backPushButton_clicked();
    void on_importWalletPushButton_clicked();

    void on_walletNameLineEdit_textChanged(const QString &arg1);

private:
    Ui::PageImportWallet *ui;
    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerLabelQRectBack;
    QFont headerLabelQFontBack;
    QRect walletNameLineEditQRectBack;
    QFont walletNameLineEditQFontBack;
    QRect importWalletPushButtonQRectBack;
    QFont importWalletPushButtonQFontBack;
};

#endif // PAGEIMPORTWALLET_H
