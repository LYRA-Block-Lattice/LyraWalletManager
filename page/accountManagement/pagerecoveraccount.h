#ifndef PAGERECOVERACCOUNT_H
#define PAGERECOVERACCOUNT_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageRecoverAccount;
}

class PageRecoverAccount : public QWidget
{
    Q_OBJECT

public:
    explicit PageRecoverAccount(QWidget *parent = nullptr);
    ~PageRecoverAccount();
    void open();
    void setScale();
    void setStyle();
    void setVisibility(bool v);
    bool getVisibility();

private slots:
    void on_showHidePushButton_clicked();
    void on_backPushButton_clicked();
    void on_recoverAccountPushButton_clicked();

    void on_accountNameLineEdit_textChanged(const QString &arg1);

    void on_walletPasswordLineEdit_textChanged(const QString &arg1);

    void on_privateKeyLineEdit_textChanged(const QString &arg1);

private:
    Ui::PageRecoverAccount *ui;
    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerLabelQRectBack;
    QFont headerLabelQFontBack;
    QRect accountNameLineEditQRectBack;
    QFont accountNameLineEditQFontBack;
    QRect walletPasswordLineEditQRectBack;
    QFont walletPasswordLineEditQFontBack;
    QRect privateKeyLineEditQRectBack;
    QFont privateKeyLineEditQFontBack;
    QRect showHidePushButtonQRectBack;
    QSize showHidePushButtonQSizeBack;
    QRect recoverAccountPushButtonQRectBack;
    QFont recoverAccountPushButtonQFontBack;
};

#endif // PAGERECOVERACCOUNT_H
