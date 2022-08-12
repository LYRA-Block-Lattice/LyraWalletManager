#ifndef PAGESWAP_H
#define PAGESWAP_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageSwap;
}

class PageSwap : public QWidget
{
    Q_OBJECT

public:
    explicit PageSwap(QWidget *parent = nullptr);
    ~PageSwap();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_swapSelectPushButton_clicked();
    void on_poolSelectPushButton_clicked();
    void on_amountSentLineEdit_textChanged(const QString &arg1);
    void on_tokenSentComboBox_currentTextChanged(const QString &arg1);
    void on_amountReceiveLineEdit_textChanged(const QString &arg1);
    void on_tokenReceiveComboBox_currentTextChanged(const QString &arg1);
    void on_swapPushButton_clicked();
    void on_removeLiquidityPushButton_clicked();

private:
    void populateSendTickers(QString txt);
    QString sendSelectedTicker;
    QString receiveSelectedTicker;
    bool populatingTickers = false;

    Ui::PageSwap *ui;

    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;

    QRect valuesVerticalLayoutQRectBack;
    QRect swapSelectPushButtonQRectBack;
    QFont swapSelectPushButtonQFontBack;
    QRect poolSelectPushButtonQRectBack;
    QFont poolSelectPushButtonQFontBack;
    QRect swapPushButtonQRectBack;
    QFont swapPushButtonQFontBack;
    QRect removeSharePushButtonQRectBack;
    QFont removeSharePushButtonQFontBack;

    QRect amountSentLineEditQRectBack;
    QFont amountSentLineEditQFontBack;
    QRect tokenSentComboBoxQRectBack;
    QFont tokenSentComboBoxQFontBack;
    QSize tokenSentComboBoxQSizeBack;
    QRect availableSentLabelQRectBack;
    QFont availableSentLabelQFontBack;

    QRect amountReceiveLineEditQRectBack;
    QFont amountReceiveLineEditQFontBack;
    QRect tokenReceiveComboBoxQRectBack;
    QFont tokenReceiveComboBoxQFontBack;
    QSize tokenReceiveComboBoxQSizeBack;
    QRect availableReceiveLabelQRectBack;
    QFont availableReceiveLabelQFontBack;

    QRect statucGridWidgetQRectBack;
    QFont statucGridWidgetQFontBack;

    int AccountListChangedCount = -1;
};

#endif // PAGESWAP_H
