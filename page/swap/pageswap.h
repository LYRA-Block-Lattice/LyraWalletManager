#ifndef PAGESWAP_H
#define PAGESWAP_H

#include <QWidget>
#include <QTimer>
#include <QLabel>

#include <QRect>
#include <QFont>
#include <QSize>

#include "api/web/webget.h"
#include "api/web/webclass.h"

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
    void on_amountSendLineEdit_textChanged(const QString &arg1);
    void on_tokenSendComboBox_currentTextChanged(const QString &arg1);
    void on_amountReceiveLineEdit_textChanged(const QString &arg1);
    void on_tokenReceiveComboBox_currentTextChanged(const QString &arg1);
    void on_swapPushButton_clicked();
    void on_removeSharePushButton_clicked();

    void on_editTimeoutTimer();
    void on_availableSendPushButton_clicked();

private:
    void populateSendTickers(QString txt);
    bool populatingTickers = false;
    void fetchPool();
    void fetchPoolCalculate(QString poolId, QString tickerFrom, double amount, double slippage);
    void fetchHistory();
    void createPool();
    void addLiquidity();
    void removeLiquidity();
    void swap();
    void showSwapData();
    void showPoolCalculateData(bool show);

    void fetchYourShare();
    void on_yourShareFetchDone(QString data);
    void on_ourShareFetchError(QString err);

    Ui::PageSwap *ui;
    QWidget *Parent;

    WalletRpc::Pool *poolThread = nullptr;
    QThread *poolWorkerThread = nullptr;
    WalletRpc::PoolCalculate *poolCalculateThread = nullptr;
    QThread *poolCalculateWorkerThread = nullptr;
    WalletRpc::Swap *swapThread = nullptr;
    QThread *swapWorkerThread = nullptr;
    WalletRpc::History *historyThread = nullptr;
    QThread *historyWorkerThread = nullptr;
    WalletRpc::CreatePool *createPoolThread = nullptr;
    QThread *createPoolWorkerThread = nullptr;
    WalletRpc::AddLiquidity *addLiquidityThread = nullptr;
    QThread *addLiquidityWorkerThread = nullptr;
    WalletRpc::RemoveLiquidity *removeLiquidityThread = nullptr;
    QThread *removeLiquidityWorkerThread = nullptr;

    WebGet *yourShareFetchWorker = nullptr;
    QThread *yourShareFetchWorkerThread = nullptr;

    QMovie *progressMovie;
    QLabel *progressLabel;

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

    QRect amountSendLineEditQRectBack;
    QFont amountSendLineEditQFontBack;
    QRect tokenSendComboBoxQRectBack;
    QFont tokenSendComboBoxQFontBack;
    QSize tokenSendComboBoxQSizeBack;
    QRect availableSendPushButtonQRectBack;
    QFont availableSendPushButtonQFontBack;

    QRect amountReceiveLineEditQRectBack;
    QFont amountReceiveLineEditQFontBack;
    QRect tokenReceiveComboBoxQRectBack;
    QFont tokenReceiveComboBoxQFontBack;
    QSize tokenReceiveComboBoxQSizeBack;
    QRect availableReceiveLabelQRectBack;
    QFont availableReceiveLabelQFontBack;

    QRect statusGridWidgetQRectBack;
    QFont statusGridWidgetQFontBack;

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
    QRect progressMovieQRectBack;
    QRect statusLabelQRectBack;
    QFont statusLabelQFontBack;


    int AccountListChangedCount = -1;
    int WalletHistoryChangedCount = -1;
    int LyrExternPricesChangedCount = -1;
    int LyrInternPricesChangedCount = -1;
    int historyCountChanged = -1;
    int accountHistoryCountChanged = -1;
    QString sendSelectedTicker;
    QString receiveSelectedTicker;
    QString lastReceiveSelectedTicker;
    double minimumReceive;
    RpcClass::Pool *poolInstance = nullptr;
    bool poolExists = false;

    QTimer editTimeoutTimer;
signals:
    void poolStartFetch(QString token1, QString token0, QList<QString> *userData);
    void poolCalculateStartFetch(QString poolId, QString swapFrom, double amount, double slippage);
    void swapStartFetch(QString token0, QString token1, QString tokenToSwap, double amountToSwap, double amountToGet);
    void historyStartFetch();
    void yourShareStartFetch(QString url);
    void createPoolStart(QString token0, QString token1);
    void addLiquidityStart(QString token0, double token0Amount, QString token1, double token1Amount);
    void removeLiquidityStart(QString token0, QString token1);
};

#endif // PAGESWAP_H
