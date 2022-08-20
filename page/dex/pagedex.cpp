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
    });
    connect(page, &PageDexListMain::withdrawPushButton, this, [=](int index) {
        qDebug() << "Withdraw" << index;
    });

    setScale();
    setStyle();

    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
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
            QList<WebClass::DexGetAllWallets::entry_t> WalletsEntryList = dexGetAllWalletsInst.getEntryList();
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
