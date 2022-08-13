#ifndef PAGESWAP_H
#define PAGESWAP_H

#include <QWidget>
#include <QTimer>

#include <QRect>
#include <QFont>
#include <QSize>

#include "wallet/walletrpc.h"

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

    void on_editTimeoutTimer();

private:
    void populateSendTickers(QString txt);
    bool populatingTickers = false;
    void fetchPool();
    void fetchPoolCalculate(QString poolId, QString tickerFrom, QString tickerTo, double amount, double slippage);
    void showPoolCalculateData(bool show);

    Ui::PageSwap *ui;

    WalletRpc::Pool *poolThread = nullptr;
    QThread *poolWorkerThread = nullptr;
    WalletRpc::PoolCalculate *poolCalculateThread = nullptr;
    QThread *poolCalculateWorkerThread = nullptr;

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

    QFont externalPriceLabelQFontBack;
    QFont externalPriceValueLabelQFontBack;
    QFont internalPriceLabelQFontBack;
    QFont internalPriceValueLabelQFontBack;
    QFont tokenPairLabelQFontBack;
    QFont tokenPairValueLabelQFontBack;
    QFont estimatedRatioLabelQFontBack;
    QFont estimatedRatioValueLabelQFontBack;
    QFont youWillSellLabelQFontBack;
    QFont youWillSellValueLabelQFontBack;
    QFont youWillGetLabelQFontBack;
    QFont youWillGetValueLabelQFontBack;
    QFont priceImpactLabelQFontBack;
    QFont priceImpactValueLabelQFontBack;
    QFont poolFeeLabelQFontBack;
    QFont poolFeeValueLabelQFontBack;
    QFont networkFeeLabelQFontBack;
    QFont networkFeeValueLabelQFontBack;
    QFont totalLiquidityLabelQFontBack;
    QFont totalLiquidityValueLabelQFontBack;
    QFont yourShareLabelQFontBack;
    QFont yourShareValueLabelQFontBack;


    int AccountListChangedCount = -1;
    int WalletHistoryChangedCount = -1;
    int LyrExternPricesChangedCount = -1;
    int LyrInternPricesChangedCount = -1;
    QString sendSelectedTicker;
    QString receiveSelectedTicker;
    QString poolIdool;

    QTimer editTimeoutTimer;
signals:
    void poolStartFetch(QString token1, QString token0, QList<QString> *userData);
    void poolCalculateStartFetch(QString poolId, QString swapFrom, double amount, double slippage);

};

#endif // PAGESWAP_H
