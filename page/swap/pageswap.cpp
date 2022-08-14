#include "pageswap.h"
#include "ui_pageswap.h"

#include <QGraphicsOpacityEffect>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"
#include "color.h"
#include "style.h"

#include "wallet/history.h"
#include "translate/translate.h"

static QTimer fadeTimer;
static int fadeCount;

PageSwap::PageSwap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSwap) {
    ui->setupUi(this);

    progressMovie = new QMovie(":/res/ic/res/ic/waiting_light.gif");
    if (progressMovie->isValid()) {
        progressLabel = new QLabel(this);
        progressLabel->setGeometry(135,400,40,40);
        progressLabel->setScaledContents(true);
        progressLabel->setMovie(progressMovie);
        progressLabel->setVisible(false);
        //progressMovie->start();
    }

    // Backup items geometry, size and font.
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();

    valuesVerticalLayoutQRectBack = ui->valuesVerticalWidget->geometry();
    swapSelectPushButtonQRectBack = ui->swapSelectPushButton->geometry();
    swapSelectPushButtonQFontBack = ui->swapSelectPushButton->font();
    poolSelectPushButtonQRectBack = ui->poolSelectPushButton->geometry();
    poolSelectPushButtonQFontBack = ui->poolSelectPushButton->font();
    swapPushButtonQRectBack = ui->swapPushButton->geometry();
    swapPushButtonQFontBack = ui->swapPushButton->font();
    removeSharePushButtonQRectBack = ui->removeSharePushButton->geometry();
    removeSharePushButtonQFontBack = ui->removeSharePushButton->font();

    amountSendLineEditQRectBack = ui->amountSendLineEdit->geometry();
    amountSendLineEditQFontBack = ui->amountSendLineEdit->font();
    tokenSendComboBoxQRectBack = ui->tokenSendComboBox->geometry();
    tokenSendComboBoxQFontBack = ui->tokenSendComboBox->font();
    tokenSendComboBoxQSizeBack = ui->tokenSendComboBox->iconSize();
    availableSendLabelQRectBack = ui->availableSendLabel->geometry();
    availableSendLabelQFontBack = ui->availableSendLabel->font();

    amountReceiveLineEditQRectBack = ui->amountReceiveLineEdit->geometry();
    amountReceiveLineEditQFontBack = ui->amountReceiveLineEdit->font();
    tokenReceiveComboBoxQRectBack = ui->tokenReceiveComboBox->geometry();
    tokenReceiveComboBoxQFontBack = ui->tokenReceiveComboBox->font();
    tokenReceiveComboBoxQSizeBack = ui->tokenReceiveComboBox->iconSize();
    availableReceiveLabelQRectBack = ui->availableReceiveLabel->geometry();
    availableReceiveLabelQFontBack = ui->availableReceiveLabel->font();

    statucGridWidgetQRectBack = ui->statucGridWidget->geometry();
    statucGridWidgetQFontBack = ui->statucGridWidget->font();

    externalPriceLabelQFontBack = ui->externalPriceLabel->font();
    externalPriceValueLabelQFontBack = ui->externalPriceValueLabel->font();
    internalPriceLabelQFontBack = ui->internalPriceLabel->font();
    internalPriceValueLabelQFontBack = ui->internalPriceValueLabel->font();
    tokenPairLabelQFontBack = ui->tokenPairLabel->font();
    tokenPairValueLabelQFontBack = ui->tokenPairValueLabel->font();
    estimatedRatioLabelQFontBack = ui->estimatedRatioLabel->font();
    estimatedRatioValueLabelQFontBack = ui->estimatedRatioValueLabel->font();
    youWillSellLabelQFontBack = ui->youWillSellLabel->font();
    youWillSellValueLabelQFontBack = ui->youWillSellValueLabel->font();
    youWillGetLabelQFontBack = ui->youWillGetLabel->font();
    youWillGetValueLabelQFontBack = ui->youWillGetValueLabel->font();
    priceImpactLabelQFontBack = ui->priceImpactLabel->font();
    priceImpactValueLabelQFontBack = ui->priceImpactValueLabel->font();
    poolFeeLabelQFontBack = ui->poolFeeLabel->font();
    poolFeeValueLabelQFontBack = ui->poolFeeValueLabel->font();
    networkFeeLabelQFontBack = ui->networkFeeLabel->font();
    networkFeeValueLabelQFontBack = ui->networkFeeValueLabel->font();
    totalLiquidityLabelQFontBack = ui->totalLiquidityLabel->font();
    totalLiquidityValueLabelQFontBack = ui->totalLiquidityValueLabel->font();
    yourShareLabelQFontBack = ui->yourShareLabel->font();
    yourShareValueLabelQFontBack = ui->yourShareValueLabel->font();

    progressMovieQRectBack = progressLabel->geometry();

    statusLabelQRectBack = ui->statusLabel->geometry();
    statusLabelQFontBack = ui->statusLabel->font();

    setScale();
    setStyle();

    ui->tokenSendComboBox->setView(new QListView);
    ui->tokenReceiveComboBox->setView(new QListView);

    ui->statusLabel->setVisible(false);
    fadeCount = 0;
    fadeTimer.setInterval(20);
    fadeTimer.stop();
    connect(&fadeTimer, &QTimer::timeout, this, [=] {
        if(fadeCount == FADE_COUNT_START_VALE)
            ui->statusLabel->setVisible(true);
        double opacity = 0;
        if(fadeCount == 0) {
            opacity = 0.0;
            ui->statusLabel->setVisible(false);
            fadeTimer.stop();
        } else  if(fadeCount < 100) {
            opacity = (double)fadeCount / 100.0;
            fadeCount--;
        } else {
            opacity = 1.0;
            fadeCount--;
        }
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(opacity);
        ui->statusLabel->setGraphicsEffect(effect);
    });
    connect(&editTimeoutTimer, &QTimer::timeout, this, &PageSwap::on_editTimeoutTimer);
}

PageSwap::~PageSwap() {
    delete ui;
}

void PageSwap::open() {
    sendSelectedTicker = "";
    receiveSelectedTicker = "";
    historyCountChanged = Wallet::History::getChangeCount();
    Wallet::History::HistoryEntry_t *accountHistory = Wallet::History::get();
    if(accountHistory) {
        accountHistoryCountChanged = accountHistory->ChangeCount;
    }
    populateSendTickers("LYR");
    ui->removeSharePushButton->setVisible(false);
    showSwapData();
}

void PageSwap::close() {

}

void PageSwap::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->valuesVerticalWidget->setGeometry(Global::Layout::scaleRect(valuesVerticalLayoutQRectBack));
    ui->swapSelectPushButton->setGeometry(Global::Layout::scaleRect(swapSelectPushButtonQRectBack));
    ui->swapSelectPushButton->setFont(Global::Layout::scaleFontOffset(swapSelectPushButtonQFontBack));
    ui->poolSelectPushButton->setGeometry(Global::Layout::scaleRect(poolSelectPushButtonQRectBack));
    ui->poolSelectPushButton->setFont(Global::Layout::scaleFontOffset(poolSelectPushButtonQFontBack));
    ui->swapPushButton->setGeometry(Global::Layout::scaleRect(swapPushButtonQRectBack));
    ui->swapPushButton->setFont(Global::Layout::scaleFontOffset(swapPushButtonQFontBack));
    ui->removeSharePushButton->setGeometry(Global::Layout::scaleRect(removeSharePushButtonQRectBack));
    ui->removeSharePushButton->setFont(Global::Layout::scaleFontOffset(removeSharePushButtonQFontBack));

    ui->amountSendLineEdit->setGeometry(Global::Layout::scaleRect(amountSendLineEditQRectBack));
    ui->amountSendLineEdit->setFont(Global::Layout::scaleFontOffset(amountSendLineEditQFontBack));
    ui->tokenSendComboBox->setGeometry(Global::Layout::scaleRect(tokenSendComboBoxQRectBack));
    ui->tokenSendComboBox->setFont(Global::Layout::scaleFontOffset(tokenSendComboBoxQFontBack));
    ui->tokenSendComboBox->setIconSize(Global::Layout::scaleSize(tokenSendComboBoxQSizeBack));
    ui->availableSendLabel->setGeometry(Global::Layout::scaleRect(availableSendLabelQRectBack));
    ui->availableSendLabel->setFont(Global::Layout::scaleFontOffset(availableSendLabelQFontBack));

    ui->amountReceiveLineEdit->setGeometry(Global::Layout::scaleRect(amountReceiveLineEditQRectBack));
    ui->amountReceiveLineEdit->setFont(Global::Layout::scaleFontOffset(amountReceiveLineEditQFontBack));
    ui->tokenReceiveComboBox->setGeometry(Global::Layout::scaleRect(tokenReceiveComboBoxQRectBack));
    ui->tokenReceiveComboBox->setFont(Global::Layout::scaleFontOffset(tokenReceiveComboBoxQFontBack));
    ui->tokenReceiveComboBox->setIconSize(Global::Layout::scaleSize(tokenReceiveComboBoxQSizeBack));
    ui->availableReceiveLabel->setGeometry(Global::Layout::scaleRect(availableReceiveLabelQRectBack));
    ui->availableReceiveLabel->setFont(Global::Layout::scaleFontOffset(availableReceiveLabelQFontBack));

    ui->statucGridWidget->setGeometry(Global::Layout::scaleRect(statucGridWidgetQRectBack));
    ui->statucGridWidget->setFont(Global::Layout::scaleFontOffset(statucGridWidgetQFontBack));

    ui->externalPriceLabel->setFont(Global::Layout::scaleFontOffset(externalPriceLabelQFontBack));
    ui->externalPriceValueLabel->setFont(Global::Layout::scaleFontOffset(externalPriceValueLabelQFontBack));
    ui->internalPriceLabel->setFont(Global::Layout::scaleFontOffset(internalPriceLabelQFontBack));
    ui->internalPriceValueLabel->setFont(Global::Layout::scaleFontOffset(internalPriceValueLabelQFontBack));
    ui->tokenPairLabel->setFont(Global::Layout::scaleFontOffset(tokenPairLabelQFontBack));
    ui->tokenPairValueLabel->setFont(Global::Layout::scaleFontOffset(tokenPairValueLabelQFontBack));
    ui->estimatedRatioLabel->setFont(Global::Layout::scaleFontOffset(estimatedRatioLabelQFontBack));
    ui->estimatedRatioValueLabel->setFont(Global::Layout::scaleFontOffset(estimatedRatioValueLabelQFontBack));
    ui->youWillSellLabel->setFont(Global::Layout::scaleFontOffset(youWillSellLabelQFontBack));
    ui->youWillSellValueLabel->setFont(Global::Layout::scaleFontOffset(youWillSellValueLabelQFontBack));
    ui->youWillGetLabel->setFont(Global::Layout::scaleFontOffset(youWillGetLabelQFontBack));
    ui->youWillGetValueLabel->setFont(Global::Layout::scaleFontOffset(youWillGetValueLabelQFontBack));
    ui->priceImpactLabel->setFont(Global::Layout::scaleFontOffset(priceImpactLabelQFontBack));
    ui->priceImpactValueLabel->setFont(Global::Layout::scaleFontOffset(priceImpactValueLabelQFontBack));
    ui->poolFeeLabel->setFont(Global::Layout::scaleFontOffset(poolFeeLabelQFontBack));
    ui->poolFeeValueLabel->setFont(Global::Layout::scaleFontOffset(poolFeeValueLabelQFontBack));
    ui->networkFeeLabel->setFont(Global::Layout::scaleFontOffset(networkFeeLabelQFontBack));
    ui->networkFeeValueLabel->setFont(Global::Layout::scaleFontOffset(networkFeeValueLabelQFontBack));
    ui->totalLiquidityLabel->setFont(Global::Layout::scaleFontOffset(totalLiquidityLabelQFontBack));
    ui->totalLiquidityValueLabel->setFont(Global::Layout::scaleFontOffset(totalLiquidityValueLabelQFontBack));
    ui->yourShareLabel->setFont(Global::Layout::scaleFontOffset(yourShareLabelQFontBack));
    ui->yourShareValueLabel->setFont(Global::Layout::scaleFontOffset(yourShareValueLabelQFontBack));

    progressLabel->setGeometry(Global::Layout::scaleRect(progressMovieQRectBack));

    ui->statusLabel->setGeometry(Global::Layout::scaleRect(statusLabelQRectBack));
    ui->statusLabel->setFont(Global::Layout::scaleFontOffset(statusLabelQFontBack));
}

void PageSwap::setStyle() {

}

void PageSwap::loop() {
    if(LyrExternPricesChangedCount != Global::TickerPrice::getModifyCount("LYR")) {
        LyrExternPricesChangedCount = Global::TickerPrice::getModifyCount("LYR");
        ui->externalPriceValueLabel->setText(QString::asprintf("$%s", Global::Util::normaliseNumber(Global::TickerPrice::get("LYR")).toUtf8().data()));
    }
    if(LyrInternPricesChangedCount != Global::TickerPrice::getModifyCount("intern/LYR")) {
        LyrInternPricesChangedCount = Global::TickerPrice::getModifyCount("intern/LYR");
        ui->internalPriceValueLabel->setText(QString::asprintf("$%s", Global::Util::normaliseNumber(Global::TickerPrice::get("intern/LYR")).toUtf8().data()));
    }
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount() ||
            WalletHistoryChangedCount != Wallet::History::getChangeCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
        WalletHistoryChangedCount = Wallet::History::getChangeCount();
    }
    if(sendSelectedTicker.compare(ui->tokenSendComboBox->currentText()) ||
            receiveSelectedTicker.compare(ui->tokenReceiveComboBox->currentText())) {
        sendSelectedTicker = ui->tokenSendComboBox->currentText();
        receiveSelectedTicker = ui->tokenReceiveComboBox->currentText();
        fetchPool();
    }
    if(historyCountChanged != Wallet::History::getChangeCount()) {
        historyCountChanged = Wallet::History::getChangeCount();
        Wallet::History::HistoryEntry_t *accountHistory = Wallet::History::get();
        if(accountHistoryCountChanged != accountHistory->ChangeCount) {
            accountHistoryCountChanged = accountHistory->ChangeCount;
            ui->availableSendLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(ui->tokenSendComboBox->currentText())).toUtf8().data()));
            ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(ui->tokenReceiveComboBox->currentText())).toUtf8().data()));
        }
    }
}

void PageSwap::populateSendTickers(QString txt) {
    populatingTickers = true;
    QList<QString> availableSwapTicker = Global::Swap::getAvailableTickers();
    if(!txt.compare("LYR")) {
        ui->tokenSendComboBox->clear();
        ui->tokenSendComboBox->addItem(QPixmap(Global::TickerIcon::get("LYR")), "LYR");
        ui->tokenReceiveComboBox->clear();
        bool first = true;
        foreach(QString swapTicker, availableSwapTicker) {
            ui->tokenReceiveComboBox->addItem(QPixmap(Global::TickerIcon::get(swapTicker)), swapTicker);
            if(swapTicker.compare("LYR") && first) {
                first = false;
                ui->tokenReceiveComboBox->setCurrentText(swapTicker);
                lastReceiveSelectedTicker = ui->tokenReceiveComboBox->currentText();
            }
        }
        ui->availableSendLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(ui->tokenSendComboBox->currentText())).toUtf8().data()));
        ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(ui->tokenReceiveComboBox->currentText())).toUtf8().data()));
    } else {
        ui->availableSendLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(txt)).toUtf8().data()));
    }
    populatingTickers = false;
}

void PageSwap::fetchPool() {
    ui->totalLiquidityValueLabel->setText(QString::asprintf("0 %s\n\r0 %s",
                                                            ui->tokenSendComboBox->currentText().toUtf8().data(),
                                                            ui->tokenReceiveComboBox->currentText().toUtf8().data()));
    ui->amountReceiveLineEdit->setText("");
    showPoolCalculateData(false);
    if(!ui->tokenSendComboBox->currentText().compare(ui->tokenReceiveComboBox->currentText()))
        return;
    fetchYourShare();
    progressMovie->start();
    progressLabel->setVisible(true);
    poolThread = new WalletRpc::Pool;
    poolWorkerThread = new QThread;
    poolThread->moveToThread(poolWorkerThread);
    connect(poolWorkerThread, &QThread::finished, poolThread, &QObject::deleteLater);
    connect(this, &PageSwap::poolStartFetch, poolThread, &WalletRpc::Pool::doWork);
    connect(poolThread, &WalletRpc::Pool::resultReady, this, [=](const QString d, QList<QString> userData) {
        if(poolInstance)
            delete poolInstance;
        poolInstance = new RpcClass::Pool(d);
        poolExists = poolInstance->getValid();
        if(!ui->poolSelectPushButton->isEnabled()) {
            bool receiveEditorEnable = !poolExists || (poolInstance->getBalanceToken0() == 0.0 && poolInstance->getBalanceToken1() == 0.0);
            ui->amountReceiveLineEdit->setEnabled(receiveEditorEnable);
            ui->amountReceiveLineEdit->setPlaceholderText(receiveEditorEnable ? Tr("Amount") : "");
        } else {
            ui->amountReceiveLineEdit->setEnabled(false);
        }
        if(poolInstance->getValid()) {
            ui->totalLiquidityValueLabel->setText(QString::asprintf("%s %s\n\r%s %s",
                                                                    Global::Util::normaliseNumber(poolInstance->getBalanceToken0()).toUtf8().data(),
                                                                    Global::Util::tickerToSign(poolInstance->getToken0()).toUtf8().data(),
                                                                    Global::Util::normaliseNumber(poolInstance->getBalanceToken1()).toUtf8().data(),
                                                                    Global::Util::tickerToSign(poolInstance->getToken1()).toUtf8().data()));
            if(!userData[0].compare("PoolCalculate")) {
                fetchPoolCalculate(poolInstance->getPoolId(), userData[1], userData[2].toDouble(), userData[3].toDouble());
            }
        }
    });
    poolWorkerThread->start();
    emit poolStartFetch(Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                        Global::Util::signToTicker(ui->tokenReceiveComboBox->currentText()),
                        new QList<QString>({"PoolCalculate",
                                            Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                                            ui->amountSendLineEdit->text().remove(','),
                                            "0.0001"})
                        );
}

void PageSwap::fetchPoolCalculate(QString poolId, QString tickerFrom, double amount, double slippage) {
    if(amount == 0) {
        progressMovie->stop();
        progressLabel->setVisible(false);
        return;
    }
    poolCalculateThread = new WalletRpc::PoolCalculate;
    poolCalculateWorkerThread = new QThread;
    poolCalculateThread->moveToThread(poolCalculateWorkerThread);
    connect(poolCalculateWorkerThread, &QThread::finished, poolCalculateThread, &QObject::deleteLater);
    connect(this, &PageSwap::poolCalculateStartFetch, poolCalculateThread, &WalletRpc::PoolCalculate::doWork);
    connect(poolCalculateThread, &WalletRpc::PoolCalculate::resultReady, this, [=](const QString d) {
        RpcClass::PoolCalculate poolCalculateInstance = RpcClass::PoolCalculate(d);
        if(poolCalculateInstance.getValid()) {
            ui->tokenPairValueLabel->setText(QString::asprintf("%s vs %s",
                                                               Global::Util::tickerToSign(poolCalculateInstance.getSwapInToken()).toUtf8().data(),
                                                               Global::Util::tickerToSign(poolCalculateInstance.getSwapOutToken()).toUtf8().data())
                                             );
            ui->estimatedRatioValueLabel->setText(QString::asprintf("%s %s per %s",
                                                                    Global::Util::normaliseNumber(poolCalculateInstance.getPrice()).toUtf8().data(),
                                                                    Global::Util::tickerToSign(poolCalculateInstance.getSwapInToken()).toUtf8().data(),
                                                                    Global::Util::tickerToSign(poolCalculateInstance.getSwapOutToken()).toUtf8().data())
                                                  );
            ui->youWillSellValueLabel->setText(QString::asprintf("%s %s",
                                                                 Global::Util::normaliseNumber(poolCalculateInstance.getSwapInAmount()).toUtf8().data(),
                                                                 poolCalculateInstance.getSwapInToken().toUtf8().data())
                                               );
            ui->youWillGetValueLabel->setText(QString::asprintf("%s %s",
                                                                 Global::Util::normaliseNumber(poolCalculateInstance.getSwapOutAmount()).toUtf8().data(),
                                                                 Global::Util::tickerToSign(poolCalculateInstance.getSwapOutToken()).toUtf8().data())
                                               );
            ui->priceImpactValueLabel->setText(QString::asprintf("%s%%",
                                                                 Global::Util::normaliseNumber(poolCalculateInstance.getPriceImpact() * 100).toUtf8().data())
                                               );
            ui->poolFeeValueLabel->setText(QString::asprintf("%s %s",
                                                             Global::Util::normaliseNumber(poolCalculateInstance.getPayToProvider()).toUtf8().data(),
                                                             Global::Util::tickerToSign(poolCalculateInstance.getSwapInToken()).toUtf8().data())
                                           );
            ui->networkFeeValueLabel->setText(QString::asprintf("%s LYR",
                                                                Global::Util::normaliseNumber(poolCalculateInstance.getPayToAuthorizer()).toUtf8().data())
                                              );
            if (ui->swapSelectPushButton->isEnabled()) { // Pool
                double multiplier = poolInstance->getBalanceToken1() / poolInstance->getBalanceToken0();
                if(!poolCalculateInstance.getSwapInToken().compare(poolInstance->getToken1()))
                    multiplier = poolInstance->getBalanceToken0() / poolInstance->getBalanceToken1();
                ui->amountReceiveLineEdit->setText(Global::Util::normaliseNumber(poolCalculateInstance.getSwapInAmount() * (multiplier)));
                showPoolCalculateData(false);
            } else { // Swap
                ui->amountReceiveLineEdit->setText(Global::Util::normaliseNumber(poolCalculateInstance.getSwapOutAmount()));
                showPoolCalculateData(true);
            }
            minimumReceive = poolCalculateInstance.getMinimumReceived();
            progressMovie->stop();
            progressLabel->setVisible(false);
        }
    });
    poolCalculateWorkerThread->start();
    emit poolCalculateStartFetch(poolId, tickerFrom, amount, slippage);
}

void PageSwap::fetchHistory() {
    historyThread = new WalletRpc::History;
    historyWorkerThread = new QThread;
    historyThread->moveToThread(historyWorkerThread);
    connect(historyWorkerThread, &QThread::finished, historyThread, &QObject::deleteLater);
    connect(this, &PageSwap::historyStartFetch, historyThread, &WalletRpc::History::doWork);
    connect(historyThread, &WalletRpc::History::resultReady, this, [=](const QString d) {
        progressLabel->setVisible(false);
        progressMovie->stop();
        RpcClass::History *historyInst = new RpcClass::History(d);
        if(historyInst->getHistory().count() > 0 && historyInst->getValid()) {
            Wallet::History::set(historyInst);
        }
    });
    connect(historyThread, &WalletRpc::History::resultError, this, [=] {
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    historyWorkerThread->start();
    emit historyStartFetch();
}

void PageSwap::fetchYourShare() {
    ui->yourShareValueLabel->setText("...");
    yourShareFetchWorker = new WebGet;
    yourShareFetchWorkerThread = new QThread;
    yourShareFetchWorker->moveToThread(yourShareFetchWorkerThread);
    connect(yourShareFetchWorker, &WebGet::resultReady, this, [=](QString data) {
        WebClass::YourShareInPool yourShare = WebClass::YourShareInPool(data);
        if(yourShare.getValid()) {
            double accShareValue = -0.0;
            QPair<QString, double> share;
            foreach (share, yourShare.getShares()) {
                if(!share.first.compare(Global::Account::getAccountPublicId())) {
                    accShareValue = share.second;
                    ui->yourShareValueLabel->setText(QString::asprintf("%s%%", Global::Util::normaliseNumber(accShareValue).toUtf8().data()));
                    return;
                }
            }
            ui->yourShareValueLabel->setText(Tr("No share"));
            progressMovie->stop();
            progressLabel->setVisible(false);
        }
    });
    connect(yourShareFetchWorker, &WebGet::resultError, this, [=](QString err) {
        Q_UNUSED(err)
    });
    connect(yourShareFetchWorkerThread, &QThread::finished, yourShareFetchWorker, &QObject::deleteLater);
    connect(this, &PageSwap::yourShareStartFetch, yourShareFetchWorker, &WebGet::doWork);
    yourShareFetchWorkerThread->start();
    emit yourShareStartFetch("https://" + Global::Network::getNodeAddress().second +
                             LYRA_NODE_API_URL +
                             "/GetPool/?token0=" +
                             Global::Util::signToTicker(ui->tokenSendComboBox->currentText()) +
                             "&token1=" +
                             Global::Util::signToTicker(ui->tokenReceiveComboBox->currentText())
                             );
}

void PageSwap::createPool() {
    ui->statusLabel->setText(Tr("Creating pool"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    createPoolThread = new WalletRpc::CreatePool;
    createPoolWorkerThread = new QThread;
    createPoolThread->moveToThread(createPoolWorkerThread);
    connect(createPoolWorkerThread, &QThread::finished, createPoolThread, &QObject::deleteLater);
    connect(this, &PageSwap::createPoolStart, createPoolThread, &WalletRpc::CreatePool::doWork);
    connect(createPoolThread, &WalletRpc::CreatePool::resultReady, this, [=](const QString d) {
        progressLabel->setVisible(false);
        progressMovie->stop();
        RpcClass::CreatePool createPoolInst = RpcClass::CreatePool(d);
        if(createPoolInst.getValid()) {
            addLiquidity();
        }
    });
    connect(createPoolThread, &WalletRpc::CreatePool::resultError, this, [=] {
        ui->statusLabel->setText(Tr("Creating pool failed"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    createPoolWorkerThread->start();
    emit createPoolStart(Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                         Global::Util::signToTicker(ui->tokenReceiveComboBox->currentText()));
}

void PageSwap::addLiquidity() {
    ui->statusLabel->setText(Tr("Adding liquidity"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    addLiquidityThread = new WalletRpc::AddLiquidity;
    addLiquidityWorkerThread = new QThread;
    addLiquidityThread->moveToThread(addLiquidityWorkerThread);
    connect(addLiquidityWorkerThread, &QThread::finished, addLiquidityThread, &QObject::deleteLater);
    connect(this, &PageSwap::addLiquidityStart, addLiquidityThread, &WalletRpc::AddLiquidity::doWork);
    connect(addLiquidityThread, &WalletRpc::AddLiquidity::resultReady, this, [=](const QString d) {
        progressLabel->setVisible(false);
        progressMovie->stop();
        RpcClass::AddLiquidity addLiquidityInst = RpcClass::AddLiquidity(d);
        if(addLiquidityInst.getValid()) {
            ui->statusLabel->setText(Tr("Adding liquidity successfull"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            fetchPool();
            fetchHistory();
            ui->amountSendLineEdit->setText("");
            ui->amountReceiveLineEdit->setText("");
        }
    });
    connect(addLiquidityThread, &WalletRpc::AddLiquidity::resultError, this, [=] {
        ui->statusLabel->setText(Tr("Adding liquidity failed"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    addLiquidityWorkerThread->start();
    emit addLiquidityStart(Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                           ui->amountSendLineEdit->text().remove(',').toDouble(),
                           Global::Util::signToTicker(ui->tokenReceiveComboBox->currentText()),
                           ui->amountReceiveLineEdit->text().remove(',').toDouble());
}

void PageSwap::removeLiquidity() {
    ui->statusLabel->setText(Tr("Removing liquidity"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    removeLiquidityThread = new WalletRpc::RemoveLiquidity;
    removeLiquidityWorkerThread = new QThread;
    removeLiquidityThread->moveToThread(removeLiquidityWorkerThread);
    connect(removeLiquidityWorkerThread, &QThread::finished, removeLiquidityThread, &QObject::deleteLater);
    connect(this, &PageSwap::createPoolStart, removeLiquidityThread, &WalletRpc::RemoveLiquidity::doWork);
    connect(removeLiquidityThread, &WalletRpc::RemoveLiquidity::resultReady, this, [=](const QString d) {
        progressLabel->setVisible(false);
        progressMovie->stop();
        RpcClass::RemoveLiquidity removePoolInst = RpcClass::RemoveLiquidity(d);
        if(removePoolInst.getValid()) {
            fetchHistory();
            fetchPool();
            ui->statusLabel->setText(Tr("Liquidity removed"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            ui->amountSendLineEdit->setText("");
            ui->amountReceiveLineEdit->setText("");
        }
    });
    connect(removeLiquidityThread, &WalletRpc::RemoveLiquidity::resultError, this, [=] {
        ui->statusLabel->setText(Tr("Removing liquidity failed"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    removeLiquidityWorkerThread->start();
    emit createPoolStart(Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                         Global::Util::signToTicker(ui->tokenReceiveComboBox->currentText()));
}

void PageSwap::swap() {
    ui->statusLabel->setText("Sending");
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    swapThread = new WalletRpc::Swap;
    swapWorkerThread = new QThread;
    swapThread->moveToThread(swapWorkerThread);
    connect(swapWorkerThread, &QThread::finished, swapThread, &QObject::deleteLater);
    connect(this, &PageSwap::swapStartFetch, swapThread, &WalletRpc::Swap::doWork);
    connect(swapThread, &WalletRpc::Swap::resultReady, this, [=](const QString d) {
        Q_UNUSED(d)
        fetchHistory();
        fetchPool();
        ui->statusLabel->setText(Tr("SWAP completed"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        ui->amountSendLineEdit->setText("");
        ui->amountReceiveLineEdit->setText("");
    });
    connect(swapThread, &WalletRpc::Swap::resultError, this, [=](QString d) {
        ui->statusLabel->setText(d);
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    swapWorkerThread->start();
    emit swapStartFetch(
                Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                Global::Util::signToTicker(ui->tokenReceiveComboBox->currentText()),
                Global::Util::signToTicker(ui->tokenSendComboBox->currentText()),
                ui->amountSendLineEdit->text().remove(',').toDouble(),
                minimumReceive
                );
}

void PageSwap::showPoolCalculateData(bool show) {
    ui->tokenPairLabel->setVisible(show);
    ui->tokenPairValueLabel->setVisible(show);

    ui->estimatedRatioLabel->setVisible(show);
    ui->estimatedRatioValueLabel->setVisible(show);

    ui->youWillSellLabel->setVisible(show);
    ui->youWillSellValueLabel->setVisible(show);

    ui->youWillGetLabel->setVisible(show);
    ui->youWillGetValueLabel->setVisible(show);

    ui->priceImpactLabel->setVisible(show);
    ui->priceImpactValueLabel->setVisible(show);

    ui->poolFeeLabel->setVisible(show);
    ui->poolFeeValueLabel->setVisible(show);

    ui->networkFeeLabel->setVisible(show);
    ui->networkFeeValueLabel->setVisible(show);
}

void PageSwap::showSwapData() {
    ui->swapSelectPushButton->setEnabled(false);
    ui->poolSelectPushButton->setEnabled(true);

    ui->externalPriceLabel->setVisible(true);
    ui->externalPriceValueLabel->setVisible(true);

    ui->internalPriceLabel->setVisible(true);
    ui->internalPriceValueLabel->setVisible(true);

    ui->tokenPairLabel->setVisible(true);
    ui->tokenPairValueLabel->setVisible(true);

    ui->estimatedRatioLabel->setVisible(true);
    ui->estimatedRatioValueLabel->setVisible(true);

    ui->youWillSellLabel->setVisible(true);
    ui->youWillSellValueLabel->setVisible(true);

    ui->youWillGetLabel->setVisible(true);
    ui->youWillGetValueLabel->setVisible(true);

    ui->priceImpactLabel->setVisible(true);
    ui->priceImpactValueLabel->setVisible(true);

    ui->poolFeeLabel->setVisible(true);
    ui->poolFeeValueLabel->setVisible(true);

    ui->networkFeeLabel->setVisible(true);
    ui->networkFeeValueLabel->setVisible(true);

    ui->swapPushButton->setText(Tr("START SWAP"));
    ui->removeSharePushButton->setVisible(false);

    //ui->amountReceiveLineEdit->setEnabled(false);
    //ui->amountReceiveLineEdit->setPlaceholderText("");
}

void PageSwap::on_swapSelectPushButton_clicked() {
    showSwapData();
    fetchPool();
}

void PageSwap::on_poolSelectPushButton_clicked() {
    ui->poolSelectPushButton->setEnabled(false);
    ui->swapSelectPushButton->setEnabled(true);

    ui->externalPriceLabel->setVisible(false);
    ui->externalPriceValueLabel->setVisible(false);

    ui->internalPriceLabel->setVisible(false);
    ui->internalPriceValueLabel->setVisible(false);

    ui->tokenPairLabel->setVisible(false);
    ui->tokenPairValueLabel->setVisible(false);

    ui->estimatedRatioLabel->setVisible(false);
    ui->estimatedRatioValueLabel->setVisible(false);

    ui->youWillSellLabel->setVisible(false);
    ui->youWillSellValueLabel->setVisible(false);

    ui->youWillGetLabel->setVisible(false);
    ui->youWillGetValueLabel->setVisible(false);

    ui->priceImpactLabel->setVisible(false);
    ui->priceImpactValueLabel->setVisible(false);

    ui->poolFeeLabel->setVisible(false);
    ui->poolFeeValueLabel->setVisible(false);

    ui->networkFeeLabel->setVisible(false);
    ui->networkFeeValueLabel->setVisible(false);

    ui->swapPushButton->setText(Tr("ADD LIQUIDITY"));
    ui->removeSharePushButton->setVisible(true);

    //ui->amountReceiveLineEdit->setEnabled(true);
    //ui->amountReceiveLineEdit->setPlaceholderText(Tr("Amount"));
    fetchPool();
}

void PageSwap::on_amountSendLineEdit_textChanged(const QString &arg1) {
    ui->amountSendLineEdit->style()->polish(ui->amountSendLineEdit);
    QString s = arg1;
    bool validate;
    s.remove(',').toDouble(&validate);
    if(s.length() > 0) {
        if(!validate) {
            ui->amountSendLineEdit->setText(Global::Util::normaliseNumber(s.left(s.length() - 1), false));
        } else {
            ui->amountSendLineEdit->setText(Global::Util::normaliseNumber(s.remove(','), false));
        }
    }
    editTimeoutTimer.stop();
    editTimeoutTimer.setInterval(2500);
    editTimeoutTimer.start();
}

void PageSwap::on_tokenSendComboBox_currentTextChanged(const QString &arg1) {
    if(populatingTickers)
        return;
    populateSendTickers(arg1);
}

void PageSwap::on_amountReceiveLineEdit_textChanged(const QString &arg1) {
    ui->amountReceiveLineEdit->style()->polish(ui->amountReceiveLineEdit);
    QString s = arg1;
    bool validate;
    s.remove(',').toDouble(&validate);
    if(s.length() > 0) {
        if(!validate) {
            ui->amountReceiveLineEdit->setText(Global::Util::normaliseNumber(s.left(s.length() - 1), false));
        } else {
            ui->amountReceiveLineEdit->setText(Global::Util::normaliseNumber(s.remove(','), false));
        }
    }
}

void PageSwap::on_tokenReceiveComboBox_currentTextChanged(const QString &arg1) {
    if(populatingTickers)
        return;
    populatingTickers = true;
    bool first = true;
    if(!arg1.compare("LYR")) {
        QList<QString> availableTickers = Wallet::History::getAvailableTikers();
        if(availableTickers.count() <= 1) {
            ui->tokenReceiveComboBox->setCurrentText(lastReceiveSelectedTicker);
            populatingTickers = false;
            return;
        }
        ui->tokenReceiveComboBox->clear();
        ui->tokenReceiveComboBox->addItem(QPixmap(Global::TickerIcon::get("LYR")), "LYR");
        ui->tokenSendComboBox->clear();
        foreach(QString ticker,availableTickers) {
            QString formatedTicker = Global::Util::tickerToSign(ticker);
            ui->tokenSendComboBox->addItem(QPixmap(Global::TickerIcon::get(formatedTicker)), formatedTicker);
            if(formatedTicker.compare("LYR")) {
                if(first) {
                    first = false;
                    ui->tokenSendComboBox->setCurrentText(formatedTicker);
                }
            }
        }
        ui->availableSendLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(ui->tokenSendComboBox->currentText())).toUtf8().data()));
        ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(ui->tokenReceiveComboBox->currentText())).toUtf8().data()));
    } else {
        ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", Tr("Balance").toUtf8().data(), Global::Util::normaliseNumber(Wallet::History::getTickerBalance(arg1)).toUtf8().data()));
    }
    lastReceiveSelectedTicker = ui->tokenReceiveComboBox->currentText();
    populatingTickers = false;
}

void PageSwap::on_editTimeoutTimer() {
    editTimeoutTimer.stop();
    fetchPool();
}

void PageSwap::on_swapPushButton_clicked() {
    if(!ui->swapSelectPushButton->isEnabled()) {
        QString sendreview = Tr("Do you want to swap") + ":\n" +
                ui->amountSendLineEdit->text() + " " + ui->tokenSendComboBox->currentText() + "\n" +
                Tr("to") + "\n" +
                ui->amountReceiveLineEdit->text() + " " + ui->tokenReceiveComboBox->currentText() ;
        QMessageBox* const message = new QMessageBox(QMessageBox::Icon::Question, Tr("Send") + "?",
            sendreview, QMessageBox::Button::Yes | QMessageBox::Button::No, this);
        message->setDefaultButton(QMessageBox::Button::No);
        message->setParent(this);
        message->open();
        connect(message, &QDialog::finished, this, [message, this] {
            message->deleteLater();
            if (message->result() == QMessageBox::Button::Yes) {
                swap();
            }
        });
    } else if(!ui->poolSelectPushButton->isEnabled()) {
        QString sendreview = Tr("Do you want to add") + ":\n" +
                ui->amountSendLineEdit->text() + " " + ui->tokenSendComboBox->currentText() + "\n" +
                Tr("and") + "\n" +
                ui->amountReceiveLineEdit->text() + " " + ui->tokenReceiveComboBox->currentText() + "\n" +
                Tr("to this pair pool") + "?";
        QMessageBox* const message = new QMessageBox(QMessageBox::Icon::Question, tr("Send") + "?",
            sendreview, QMessageBox::Button::Yes | QMessageBox::Button::No, this);
        message->setDefaultButton(QMessageBox::Button::No);
        message->setParent(this);
        message->open();
        connect(message, &QDialog::finished, this, [message, this] {
            message->deleteLater();
            if (message->result() == QMessageBox::Button::Yes) {
                ui->statusLabel->setText(Tr("Sending"));
                fadeCount = FADE_COUNT_START_VALE;
                fadeTimer.start();
                if(poolExists)
                    addLiquidity();
                else
                    createPool();
            }
        });
    }
}


void PageSwap::on_removeSharePushButton_clicked() {
    QString sendreview = Tr("You will remove liquidity from") + ":\n" +
            ui->tokenSendComboBox->currentText() + "\n" +
            ui->tokenReceiveComboBox->currentText() + "\n" +
            Tr("pair");
    QMessageBox* const message = new QMessageBox(QMessageBox::Icon::Question, Tr("Remove") + "?",
        sendreview, QMessageBox::Button::Yes | QMessageBox::Button::No, this);
    message->setDefaultButton(QMessageBox::Button::No);
    message->setParent(this);
    message->open();
    connect(message, &QDialog::finished, this, [message, this] {
        message->deleteLater();
        if (message->result() == QMessageBox::Button::Yes) {
            removeLiquidity();
        }
    });
}

