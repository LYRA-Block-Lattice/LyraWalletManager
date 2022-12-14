#ifndef PAGEACCOUNT_H
#define PAGEACCOUNT_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>
#include <QMovie>
#include <QLabel>
#include <QTimer>

#include "pageaccountlist/pageaccountlist.h"
#include "page/account/subpage/history/pagehistory.h"
#include "wallet/walletrpc.h"

namespace Ui {
class PageAccount;
}

class PageAccount : public QWidget {
    Q_OBJECT

public:
    explicit PageAccount(QWidget *parent = nullptr);
    ~PageAccount();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();
private slots:
    void on_sendPushButton_clicked();
    void on_receivePushButton_clicked();
    void on_historyPushButton_clicked();

private:
    void getBalance();
    void refreshBalances(RpcClass::History *history);
    void fetchHistory();

    Ui::PageAccount *ui;
    QWidget *parent;
    PageAccountList *messageList;
    QMovie *progressMovie;
    QLabel *progressLabel;

    QTimer refreshTimer;

    WalletRpc::Balance *balanceThread = nullptr;
    QThread *balanceWorkerThread = nullptr;
    WalletRpc::History *historyThread = nullptr;
    QThread *historyWorkerThread = nullptr;
    WalletRpc::Receive *receiveThread = nullptr;
    QThread *receiveWorkerThread = nullptr;

    QRect lyraLogoLabelQRectBack;
    QSize lyraLogoLabelQSizeBack;
    QRect accountValueLyrLabelQRectBack;
    QFont accountValueLyrLabelQFontBack;
    QRect accountValueUsdLabelQRectBack;
    QFont accountValueUsdLabelQFontBack;
    QRect accountTotalValueUsdLabelQRectBack;
    QFont accountTotalValueUsdLabelQFontBack;
    QRect sendPushButtonQRectBack;
    QSize sendPushButtonQSizeBack;
    QRect historyPushButtonQRectBack;
    QSize historyPushButtonQSizeBack;
    QRect receivePushButtonQRectBack;
    QSize receivePushButtonQSizeBack;
    QRect historyLabelQRectBack;
    QFont historyLabelQFontBack;
    QRect progressMovieQRectBack;
    QRect messageListQRectBack;

    int AccountListChangedCount = -1;
    int SelectedAccount = -1;
    int BalancesChangedCount = -1;
    networkName_e network = networkName_e::NONE;

signals:
    void balanceStartFetch();
    void historyStartFetch();
    void receiveStartFetch();
};

#endif // PAGEACCOUNT_H
