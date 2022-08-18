#include "pageaccount.h"
#include "ui_pageaccount.h"

#include <QListWidgetItem>
#include <QListView>
#include <QVBoxLayout>
#include <QTimer>

#include "style.h"
#include "global.h"

#include "wallet/history.h"

#define ACCOUNT_REFRESH_INTERVAL    (30*1000)

PageAccount::PageAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageAccount) {
    this->parent = parent;
    setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->setupUi(this);
    setStyle();

    messageList = new PageAccountList(this);

    messageList->setGeometry(QRect(0, Global::Layout::scaleValueX(220), parent->geometry().width(), parent->geometry().height() - Global::Layout::scaleValueX(220) - Global::Layout::scaleValueX(MENU_BAR_HEIGHT + 10)));

    progressMovie = new QMovie(":/res/ic/res/ic/waiting_light.gif");
    if (progressMovie->isValid()) {
        progressLabel = new QLabel(this);
        progressLabel->setGeometry(135,320,40,40);
        progressLabel->setScaledContents(true);
        progressLabel->setMovie(progressMovie);
        progressLabel->setVisible(false);
        //progressMovie->start();
    }

    // Backup items geometry, size and font.
    lyraLogoLabelQRectBack = ui->lyraLogoLabel->geometry();
    //lyraLogoLabelQSizeBack = ui->lyraLogoLabel->pixmap().size();
    accountValueLyrLabelQRectBack = ui->accountValueLyrLabel->geometry();
    accountValueLyrLabelQFontBack = ui->accountValueLyrLabel->font();
    accountValueUsdLabelQRectBack = ui->accountValueUsdLabel->geometry();
    accountValueUsdLabelQFontBack = ui->accountValueUsdLabel->font();
    accountTotalValueUsdLabelQRectBack = ui->accountTotalValueUsdLabel->geometry();
    accountTotalValueUsdLabelQFontBack = ui->accountTotalValueUsdLabel->font();
    sendPushButtonQRectBack = ui->sendPushButton->geometry();
    sendPushButtonQSizeBack = ui->sendPushButton->iconSize();
    historyPushButtonQRectBack = ui->historyPushButton->geometry();
    historyPushButtonQSizeBack = ui->historyPushButton->iconSize();
    receivePushButtonQRectBack = ui->receivePushButton->geometry();
    receivePushButtonQSizeBack = ui->receivePushButton->iconSize();
    historyLabelQRectBack = ui->historyLabel->geometry();
    historyLabelQFontBack = ui->historyLabel->font();
    progressMovieQRectBack = progressLabel->geometry();
    messageListQRectBack = messageList->geometry();

    setScale();


    messageList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    messageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    messageList->scrollToTop();
    messageList->setEnabled(true);
    //setGeometry(Global::Layout::getTabLayoutGeometryScaled());

    connect(ui->sendPushButton, &QPushButton::clicked, ui->sendPushButton, &QPushButton::setDisabled);
    connect(ui->receivePushButton, &QPushButton::clicked, ui->receivePushButton, &QPushButton::clearFocus);

    refreshTimer.setInterval(ACCOUNT_REFRESH_INTERVAL);
    connect(&refreshTimer, &QTimer::timeout, this, [=] {
        getBalance();
    });
    refreshTimer.start();
}

PageAccount::~PageAccount() {
    delete ui;
}

void PageAccount::open() {
}

void PageAccount::close() {

}

void PageAccount::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->lyraLogoLabel->setGeometry(Global::Layout::scaleRect(lyraLogoLabelQRectBack));
    //ui->lyraLogoLabel->setIconSize(Global::Layout::scaleSize(lyraLogoLabelQSizeBack));
    ui->accountValueLyrLabel->setGeometry(Global::Layout::scaleRect(accountValueLyrLabelQRectBack));
    ui->accountValueLyrLabel->setFont(Global::Layout::scaleFontOffset(accountValueLyrLabelQFontBack));
    ui->accountValueUsdLabel->setGeometry(Global::Layout::scaleRect(accountValueUsdLabelQRectBack));
    ui->accountValueUsdLabel->setFont(Global::Layout::scaleFontOffset(accountValueUsdLabelQFontBack));
    ui->accountTotalValueUsdLabel->setGeometry(Global::Layout::scaleRect(accountTotalValueUsdLabelQRectBack));
    ui->accountTotalValueUsdLabel->setFont(Global::Layout::scaleFontOffset(accountTotalValueUsdLabelQFontBack));
    ui->sendPushButton->setGeometry(Global::Layout::scaleRect(sendPushButtonQRectBack));
    ui->sendPushButton->setIconSize(Global::Layout::scaleSize(sendPushButtonQSizeBack));
    ui->historyPushButton->setGeometry(Global::Layout::scaleRect(historyPushButtonQRectBack));
    ui->historyPushButton->setIconSize(Global::Layout::scaleSize(historyPushButtonQSizeBack));
    ui->receivePushButton->setGeometry(Global::Layout::scaleRect(receivePushButtonQRectBack));
    ui->receivePushButton->setIconSize(Global::Layout::scaleSize(receivePushButtonQSizeBack));
    ui->historyLabel->setGeometry(Global::Layout::scaleRect(historyLabelQRectBack));
    ui->historyLabel->setFont(Global::Layout::scaleFontOffset(historyLabelQFontBack, Global::Layout::labelFontScale()));
    progressLabel->setGeometry(Global::Layout::scaleRect(progressMovieQRectBack));
    messageList->setGeometry(QRect(0, Global::Layout::scaleValueX(220), parent->geometry().width(),
                                   parent->geometry().height() - Global::Layout::scaleValueX(220) - Global::Layout::scaleValueX(MENU_BAR_HEIGHT + 6)));
}

void PageAccount::setStyle() {
    //Style::setButtonTransparentStyle(ui->lyraLogoLabel);
    Style::setLabelStyle(ui->accountValueLyrLabel);
    Style::setLabelStyle(ui->accountValueUsdLabel);
    Style::setLabelStyle(ui->accountTotalValueUsdLabel);
    Style::setButtonSendReceiveStyle(ui->sendPushButton);
    Style::setButtonTransparentStyle(ui->historyPushButton);
    Style::setButtonSendReceiveStyle(ui->receivePushButton);
    Style::setLabelStyle(ui->historyLabel);
}

void PageAccount::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount() ||
            SelectedAccount != Global::Account::getSelectedAccount() ||
            network != Global::Network::getNetwork()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
        SelectedAccount = Global::Account::getSelectedAccount();
        network = Global::Network::getNetwork();
        if(Global::Account::getAccountList().count() > 0) {
            if(Wallet::History::get()) {
                refreshBalances(Wallet::History::get()->history);
            }
            getBalance();
            refreshTimer.stop();
            refreshTimer.start();
        }
    }
    if(BalancesChangedCount != Global::TickerPrice::getModifyCount()) {
        BalancesChangedCount = Global::TickerPrice::getModifyCount();
        QList<QString> tickerList = messageList->getTickers();
        foreach(QString ticker, tickerList) {
            QString tick = ticker.compare("LYR") ? ticker : "intern/LYR";
            messageList->setTokenValue(ticker, Global::TickerPrice::get(tick.remove('$')));
        }
    }
}

void PageAccount::refreshBalances(RpcClass::History *history) {
    if(!history)
        return;
    messageList->clearAll();
    double totalValueUsd = 0.0;
    double lyrInAccount = 0.0;
    double lyrValueUsd = 0.0;
    if(history->getHistory().count() > 0 && history->getValid()) {
        QList<QPair<QString, double>> balances = history->getHistory().at(history->getHistory().count() - 1).Balances;
        QPair<QString, double> balance;
        foreach(balance, balances) {
            QString tick = balance.first.compare("LYR") ? balance.first : "intern/LYR";
            double tickerPrice = Global::TickerPrice::get(tick);
            messageList->addMessage(QPixmap(Global::TickerIcon::get(balance.first)),
                                    balance.first, balance.second,
                                    balance.second * tickerPrice,
                                    Global::TickerPrice::get(tick));
            totalValueUsd += balance.second * tickerPrice;
            if(!balance.first.compare("LYR")) {
                lyrInAccount = balance.second;
                lyrValueUsd = balance.second * tickerPrice;
            }
        }
        messageList->scrollToTop();

    }
    ui->accountValueLyrLabel->setText(Global::Util::normaliseNumber(lyrInAccount) + " LYR");
    ui->accountValueUsdLabel->setText(Global::Util::normaliseNumber(lyrValueUsd) + " USD");
    ui->accountTotalValueUsdLabel->setText("Total balance: $ " + Global::Util::normaliseNumber(totalValueUsd));
}

void PageAccount::getBalance() {
    if(Global::Account::getAccountList().count() == 0)
        return;
    balanceThread = new WalletRpc::Balance;
    balanceWorkerThread = new QThread;
    balanceThread->moveToThread(balanceWorkerThread);
    connect(balanceWorkerThread, &QThread::finished, balanceThread, &QObject::deleteLater);
    connect(this, &PageAccount::balanceStartFetch, balanceThread, &WalletRpc::Balance::doWork);
    connect(balanceThread, &WalletRpc::Balance::resultReady, this, [=](const QString d) {
        RpcClass::Balance *balanceInst = new RpcClass::Balance(d);
        if(Wallet::History::get()) {
            if(balanceInst->getUnreceived()) {
                progressMovie->start();
                progressLabel->setVisible(true);
                receiveThread = new WalletRpc::Receive;
                receiveWorkerThread = new QThread;
                receiveThread->moveToThread(receiveWorkerThread);
                connect(receiveWorkerThread, &QThread::finished, receiveThread, &QObject::deleteLater);
                connect(this, &PageAccount::receiveStartFetch, receiveThread, &WalletRpc::Receive::doWork);
                connect(receiveThread, &WalletRpc::Receive::resultReady, this, [=](const QString d) {
                    Q_UNUSED(d)
                    fetchHistory();
                });
                connect(receiveThread, &WalletRpc::Receive::resultError, this, [=] {
                    progressLabel->setVisible(false);
                    progressMovie->stop();
                });
                receiveWorkerThread->start();
                emit receiveStartFetch();
                delete balanceInst;
                return;
            }
            if(Wallet::History::get()->history->getHistory().count() == balanceInst->getHeight()) {
                progressLabel->setVisible(false);
                progressMovie->stop();
                delete balanceInst;
                return;
            }
        }
        delete balanceInst;
        progressMovie->start();
        progressLabel->setVisible(true);
        fetchHistory();
    });
    connect(balanceThread, &WalletRpc::Balance::resultError, this, [=] {
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    balanceWorkerThread->start();
    emit balanceStartFetch();
}

void PageAccount::fetchHistory() {
    historyThread = new WalletRpc::History;
    historyWorkerThread = new QThread;
    historyThread->moveToThread(historyWorkerThread);
    connect(historyWorkerThread, &QThread::finished, historyThread, &QObject::deleteLater);
    connect(this, &PageAccount::historyStartFetch, historyThread, &WalletRpc::History::doWork);
    connect(historyThread, &WalletRpc::History::resultReady, this, [=](const QString d) {
        progressLabel->setVisible(false);
        progressMovie->stop();
        RpcClass::History *historyInst = new RpcClass::History(d);
        if(historyInst->getHistory().count() > 0 && historyInst->getValid()) {
            Wallet::History::set(historyInst);
        }
        messageList->clearAll();
        refreshBalances(historyInst);
    });
    historyWorkerThread->start();
    emit historyStartFetch();
}

void PageAccount::on_sendPushButton_clicked() {
    ui->sendPushButton->clearFocus();
    Global::Page::goManagerPage(Global::Page::SEND);
}

void PageAccount::on_receivePushButton_clicked() {
    ui->receivePushButton->clearFocus();
    Global::Page::goManagerPage(Global::Page::RECEIVE);
}

void PageAccount::on_historyPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::HISTORY);
}



