#ifndef PAGECREATEACCOUNT_H
#define PAGECREATEACCOUNT_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageNewAccount;
}

class PageNewAccount : public QWidget
{
    Q_OBJECT
public:
    explicit PageNewAccount(QWidget *parent = nullptr);
    ~PageNewAccount();
    void open();
    void setScale();
    void setStyle();
    void setVisibility(bool v);
    bool getVisibility();

private slots:
    void on_showHidePushButton_clicked();
    void on_backPushButton_clicked();
    void on_createAccountPushButton_clicked();
    void on_recoverAccountPushButton_clicked();

    void on_accountNameLineEdit_textChanged(const QString &arg1);

    void on_walletPasswordLineEdit_textChanged(const QString &arg1);

private:
    Ui::PageNewAccount *ui;
    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerLabelQRectBack;
    QFont headerLabelQFontBack;
    QRect accountNameLineEditQRectBack;
    QFont accountNameLineEditQFontBack;
    QRect walletPasswordLineEditQRectBack;
    QFont walletPasswordLineEditQFontBack;
    QRect showHidePushButtonQRectBack;
    QSize showHidePushButtonQSizeBack;
    QRect createAccountPushButtonQRectBack;
    QFont createAccountPushButtonQFontBack;
    QRect recoverAccountPushButtonQRectBack;
    QFont recoverAccountPushButtonQFontBack;
};

#endif // PAGECREATEACCOUNT_H
