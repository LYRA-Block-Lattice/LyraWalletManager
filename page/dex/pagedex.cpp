#include "pagedex.h"
#include "ui_pagedex.h"

#include <QScroller>

#include "global.h"
#include "color.h"
#include "style.h"

#include "page/account/pageaccount.h"
#include "wallet/history.h"

PageDex::PageDex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDex) {
    ui->setupUi(this);
    Parent = parent;

    // Backup items geometry, size and font.
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();


    page = new PageDexListMain(this);
    ui->scrollArea->setWidget(page);

    progressMovie = new QMovie(":/res/ic/res/ic/waiting_light.gif");
    if (progressMovie->isValid()) {
        progressLabel = new QLabel(this);
        progressLabel->setGeometry(135,260,40,40);
        progressLabel->setScaledContents(true);
        progressLabel->setMovie(progressMovie);
        progressLabel->setVisible(false);
        //progressMovie->start();
    }
    progressMovieQRectBack = progressLabel->geometry();

    connect(page, &PageDexListMain::setupPushButton, this, [=](int index) {
        qDebug() << "Setup" << index;
    });
    connect(page, &PageDexListMain::spotToDexPushButton, this, [=](int index) {
        qDebug() << "Sppt to DEX" << index;
    });
    connect(page, &PageDexListMain::dexToSpotPushButton, this, [=](int index) {
        qDebug() << "DEX to Spot" << index;
    });
    connect(page, &PageDexListMain::depositPushButton, this, [=](int index) {
        qDebug() << "Deposit" << index;
        PageDexDeposit::entry_t entry;
        entry.ticker = EntryList[index].Symbol;
        entry.provider = EntryList[index].NetworkProvider;
        entry.contract = EntryList[index].Contract;
        WebClass::DexGetAllWallets::entry_t walletsEntry;
        foreach(walletsEntry, WalletsEntryList) {
            if(!walletsEntry.ExtSymbol.compare(EntryList[index].Symbol)) {
                entry.depositAddress = walletsEntry.ExtAddress;
                break;
            }
        }
        entry.minimalDeposit = EntryList[index].MinDeposit;
        entry.depositFee = EntryList[index].DepositFee;
        entry.confirmations = EntryList[index].ConfirmationInfo;
        Global::Page::goManagerPage(Global::Page::DEX_DEPOSIT, (void *)&entry);
    });
    connect(page, &PageDexListMain::withdrawPushButton, this, [=](int index) {
        qDebug() << "Withdraw" << index;
        PageDexWithdraw::entry_t entry;
        entry.ticker = EntryList[index].Symbol;
        entry.provider = EntryList[index].NetworkProvider;
        entry.contract = EntryList[index].Contract;
        entry.withdrawFee = EntryList[index].WithdrawFee;
        WebClass::DexGetAllWallets::entry_t walletsEntry;
        foreach(walletsEntry, WalletsEntryList) {
            if(!walletsEntry.ExtSymbol.compare(EntryList[index].Symbol)) {
                entry.amountInAccount = walletsEntry.Balances[0].second / 100000000;
                break;
            }
        }
        Global::Page::goManagerPage(Global::Page::DEX_WITHDRAW, (void *)&entry);
    });

    setScale();
    setStyle();

    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    sp.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    sp.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.2);
    sp.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.2);
    sp.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity, 0);
    sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
    sp.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
    sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(ui->scrollArea);
    scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
    scroller->setScrollerProperties(sp);
}

PageDex::~PageDex() {
    delete ui;
}

void PageDex::open() {

}

void PageDex::close() {

}

void PageDex::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));
    ui->scrollArea->setGeometry(QRect(0, Global::Layout::scaleValueX(56), Parent->geometry().width(), Parent->geometry().height() - Global::Layout::scaleValueX(56) - Global::Layout::scaleValueX(MENU_BAR_HEIGHT + 10)));
    progressLabel->setGeometry(Global::Layout::scaleRect(progressMovieQRectBack));
    page->setScale();
}

void PageDex::setStyle() {
    page->setStyle();
}

void PageDex::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount() ||
            SelectedAccountChangedCount != Global::Account::getSelectedAccountChangedCnt() ||
            network != Global::Network::getNetwork()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
        SelectedAccountChangedCount = Global::Account::getSelectedAccountChangedCnt();
        network = Global::Network::getNetwork();
        dexGetSupportedTokens();
    }
}

void PageDex::dexGetSupportedTokens() {
    progressLabel->setVisible(true);
    progressMovie->start();
    EntryList.clear();
    page->clear();
    dexGetSupportedTokensFetchWorker = new WebGet;
    dexGetSupportedTokensFetchWorkerThread = new QThread;
    dexGetSupportedTokensFetchWorker->moveToThread(dexGetSupportedTokensFetchWorkerThread);
    connect(dexGetSupportedTokensFetchWorker, &WebGet::resultReady, this, [=](QString s) {
        WebClass::DexGetSuportedExtTokens dexSupportedExtTokensInst = WebClass::DexGetSuportedExtTokens(s);
        if(dexSupportedExtTokensInst.getValid()) {
            EntryList = dexSupportedExtTokensInst.getEntryList();
            WebClass::DexGetSuportedExtTokens::entry_t entry;
            foreach (entry, EntryList) {
                page->append(entry.Symbol, entry.Name, Wallet::History::getTickerBalance("$" + entry.Symbol), 0);
            }
            page->repaint();
            dexGetAllWalles();
        }
        //progressLabel->setVisible(false);
        //progressMovie->stop();
    });
    connect(dexGetSupportedTokensFetchWorker, &WebGet::resultError, this, [=](QString s) {
        Q_UNUSED(s)
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    connect(dexGetSupportedTokensFetchWorkerThread, &QThread::finished, dexGetSupportedTokensFetchWorker, &QObject::deleteLater);
    connect(this, &PageDex::dexGetSupportedTokensStart, dexGetSupportedTokensFetchWorker, &WebGet::doWork);
    dexGetSupportedTokensFetchWorkerThread->start();
    emit dexGetSupportedTokensStart(Global::Network::getDexAddress() + "/GetSupportedExtToken/?networkid=" + Global::Network::getNetworkName().toLower());
}

void PageDex::dexGetAllWalles() {
    //progressLabel->setVisible(true);
    //progressMovie->start();
    dexGetAllWalletsFetchWorker = new WebGet;
    dexGetAllWalletsFetchWorkerThread = new QThread;
    dexGetAllWalletsFetchWorker->moveToThread(dexGetAllWalletsFetchWorkerThread);
    connect(dexGetAllWalletsFetchWorker, &WebGet::resultReady, this, [=](QString s) {
        WebClass::DexGetAllWallets dexGetAllWalletsInst = WebClass::DexGetAllWallets(s);
        if(dexGetAllWalletsInst.getValid()) {
            WalletsEntryList = dexGetAllWalletsInst.getEntryList();
            QList<PageDexList *> dexList = page->entryList();
            for( int i = 0; i < page->count(); i++) {
                WebClass::DexGetAllWallets::entry_t entry;
                foreach (entry, WalletsEntryList) {
                    if(!entry.IntSymbol.remove('$').compare(dexList[i]->ticker())) {
                        QList<QPair<QString, double>> balances = entry.Balances;
                        QPair<QString, double>balance = balances[0];
                        dexList[i]->setDexAmount(balance.second / 100000000);
                    }
                }
            }
        }
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    connect(dexGetAllWalletsFetchWorker, &WebGet::resultError, this, [=](QString s) {
        Q_UNUSED(s)
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    connect(dexGetAllWalletsFetchWorkerThread, &QThread::finished, dexGetAllWalletsFetchWorker, &QObject::deleteLater);
    connect(this, &PageDex::dexGetAllWalletsStart, dexGetAllWalletsFetchWorker, &WebGet::doWork);
    dexGetAllWalletsFetchWorkerThread->start();
    emit dexGetAllWalletsStart("https://" + Global::Network::getNodeAddress().second + LYRA_NODE_API_URL +
                                 "/GetAllDexWallets/?owner=" + Global::Account::getAccountPublicId());
}
