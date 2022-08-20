#ifndef PAGEDEXWITHDRAW_H
#define PAGEDEXWITHDRAW_H

#include <QWidget>

namespace Ui {
class PageDexWithdraw;
}

class PageDexWithdraw : public QWidget
{
    Q_OBJECT

public:
    explicit PageDexWithdraw(QWidget *parent = nullptr);
    ~PageDexWithdraw();

private slots:
    void on_recipientAddressLineEdit_textChanged(const QString &arg1);
    void on_qrPushButton_clicked();
    void on_amountLineEdit_textChanged(const QString &arg1);
    void on_maximumPushButton_clicked();
    void on_withdrawPushButton_clicked();
private:
    Ui::PageDexWithdraw *ui;
};

#endif // PAGEDEXWITHDRAW_H
