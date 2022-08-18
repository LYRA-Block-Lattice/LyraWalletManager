#ifndef PAGEMORE_H
#define PAGEMORE_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageMore;
}

class PageMore : public QWidget
{
    Q_OBJECT

public:
    explicit PageMore(QWidget *parent = nullptr);
    ~PageMore();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_explorerPushButton_clicked();
    void on_newAccountPushButton_clicked();
    void on_accountsComboBox_currentTextChanged(const QString &arg1);
    void on_settingsPushButton_clicked();

private:
    Ui::PageMore *ui;

    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect accountsComboBoxQRectBack;
    QSize accountsComboBoxQSizeBack;
    QFont accountsComboBoxQFontBack;
    QRect explorerPushButtonQRectBack;
    QSize explorerPushButtonQSizeBack;
    QFont explorerPushButtonQFontBack;
    QRect newAccountPushButtonQRectBack;
    QSize newAccountPushButtonQSizeBack;
    QFont newAccountPushButtonQFontBack;
    QRect settingsPushButtonQRectBack;
    QSize settingsPushButtonQSizeBack;
    QFont settingsPushButtonQFontBack;
    QRect aboutUsPushButtonQRectBack;
    QSize aboutUsPushButtonQSizeBack;
    QFont aboutUsPushButtonQFontBack;
    QRect advancedFunctionsPushButtonQRectBack;
    QSize advancedFunctionsPushButtonQSizeBack;
    QFont advancedFunctionsPushButtonQFontBack;

    int AccountListChangedCount = -1;

    bool initialized = false;

};

#endif // PAGEMORE_H
