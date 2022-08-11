#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QTimer>

#include "api/web/webget.h"
#include "api/web/webclass.h"

#include "page/accountManagement/pageimportwallet.h"
#include "page/accountManagement/pagenewaccount.h"
#include "page/accountManagement/pagenewwallet.h"
#include "page/accountManagement/pageopenwallet.h"
#include "page/accountManagement/pagerecoveraccount.h"

#include "page/staking/pagestaking.h"
#include "page/swap/pageswap.h"
#include "page/account/pageaccount.h"
#include "page/dex/pagedex.h"
#include "page/more/pagemore.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setScale();
    static void setPage(int index);
private:
    PageStaking *pageStaking;
    PageSwap *pageSwap;
    PageAccount *pageAccount;
    PageDex *pageDex;
    PageMore *pageMore;

    QRect walletNetworkNameLabelQRectBack;
    QFont walletNetworkNameLabelQFontBack;

    QTranslator * qtTranslator = NULL;

    QString accountNetworkName = " ";
    networkName_e networkName = networkName_e::NONE;

    bool initialized = false;

    void switchTranslator(const QString filename);
    void getPoolPairPrice();

    QTimer loopTimer;
    QTimer fetchCoinGeckoTimer;


private slots:
    void on_mainTabWidget_currentChanged(int index);
    void on_timerLoopTick();

    void fetchCoingecko();
    void on_coingeckoFetchDone(QString data);
    void on_coingeckoFetchError(QString err);

private:
    Ui::MainWindow *ui;

    WebGet *coinGecckoFetchWorker = nullptr;
    QThread *coinGecckoFetchWorkerThread = nullptr;

    WalletRpc::Pool *poolThread = nullptr;
    QThread *poolWorkerThread = nullptr;

    QList<QPair<QString, QString>> *priceToRetriveList = nullptr;
signals:
    void coinGeckofetch(QString url);
    void poolStartFetch(QString token1, QString token0);
};
#endif // MAINWINDOW_H
