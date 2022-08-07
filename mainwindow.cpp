#include "mainwindow.h"
#include "global.h"
#include "ui_mainwindow.h"

#include <QTranslator>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QLayout>

#include "color.h"
#include "global.h"
#include "style.h"

int lastIndex = -1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) ,
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    switchTranslator("ro");
    mainTabWidget = ui->mainTabWidget;
    this->setWindowFlags(( windowFlags() & ~(Qt::WindowFullscreenButtonHint | Qt::WindowMaximizeButtonHint)) | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    Global::Layout::setYScale(1.2);
    Global::Layout::setXScale(1.1);
    Global::Layout::setHeaderHeight(0);

    // Backup items geometry, size and font.
    walletNetworkNameLabelQRectBack = ui->walletNetworkNameLabel->geometry();
    walletNetworkNameLabelQFontBack = ui->walletNetworkNameLabel->font();

    // On mobile we need the actually available application window
#if defined(__APPLE__) || defined(Q_OS_ANDROID)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined(Q_OS_ANDROID)
    const QScreen* screen = qApp->primaryScreen();
    Global::Layout::setXScale((double)screen->availableSize().width() / (double)Global::Layout::getLayoutGeometry().width());
    Global::Layout::setYScale((double)(screen->availableSize().height()) / (double)Global::Layout::getLayoutGeometry().height());
    Global::Layout::setHeaderHeight(screen->availableVirtualGeometry().y());
    //Global::Layout::setHeaderHeight(QGuiApplication::primaryScreen()->availableSize().height() - screen->availableSize().height());
    qDebug() << "Available size: " << screen->availableSize().height() << "Layout size: " << Global::Layout::getLayoutGeometry().height();
    qDebug() << "X: " << Global::Layout::getXScale() << "Y: " << Global::Layout::getYScale();
#endif
#endif
/*#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
        const QScreen* screen = qApp->primaryScreen();
        Global::setVScale((double)screen->availableSize().width() / (double)Global::getLayoutGeometry().width());
        Global::setHScale((double)(screen->availableSize().height()) / (double)Global::getLayoutGeometry().height());
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif
#if defined(Q_OS_ANDROID)
    const QScreen* screen = qApp->primaryScreen();
    Global::setVScale((double)screen->availableSize().width() / (double)Global::getLayoutGeometry().width());
    Global::setHScale((double)(screen->availableSize().height()) / (double)Global::getLayoutGeometry().height());
#endif*/
    setScale();
    ui->mainTabWidget->repaint();

    ui->mainTabWidget->setParent(this);
    pageImportWallet = new PageImportWallet(this);
    pageNewAccount = new PageNewAccount(this);
    pageNewWallet = new PageNewWallet(this);
    pageOpenWallet = new PageOpenWallet(this);
    pageRecoverAccount = new PageRecoverAccount(this);
    pageHistory = new PageHistory(this);
    pageTransactionDetail = new PageTransactionDetail(this);
    pageSend = new PageSend(this);
    pageReceive = new PageReceive(this);

    pageImportWallet->setVisible(false);
    pageNewAccount->setVisible(false);
    pageNewWallet->setVisible(false);
    pageOpenWallet->setVisible(true);
    pageRecoverAccount->setVisible(false);
    pageHistory->setVisible(false);
    pageTransactionDetail->setVisible(false);
    pageSend->setVisible(false);
    pageReceive->setVisible(false);
    ui->mainTabWidget->setVisible(false);


    pageStaking = new PageStaking(ui->mainTabWidget);
    pageSwap = new PageSwap(ui->mainTabWidget);
    pageAccount = new PageAccount(ui->mainTabWidget);
    pageDex = new PageDex(ui->mainTabWidget);
    pageMore = new PageMore(ui->mainTabWidget);
    ui->mainTabWidget->clear();
    ui->mainTabWidget->addTab(pageStaking, tr("STAKING"));
    ui->mainTabWidget->addTab(pageSwap, tr("SWAP"));
    ui->mainTabWidget->addTab(pageAccount, tr("ACCOUNT"));
    ui->mainTabWidget->addTab(pageDex, tr("DEX"));
    ui->mainTabWidget->addTab(pageMore, tr("MORE"));
    initialized = true;
    ui->mainTabWidget->setCurrentIndex(Global::Page::PAGE::NONE);

    /*Global::Account::account_t account;
    account.accountName ="Pig2";
    account.privateKey = "";
    account.publicId = "LF6LpgcrANQWrErPcLREAbKzJg9DLeuXXa45cz5hKsUng7aJ2zCrAgHbtkSXv5dXiEfUB8ypN8i3daUkmiJwcX8cbXSv5U";
    Global::Account::addAccount(account);*/

    QPair<QString, double> tickerPrice;
    tickerPrice.first = "LYR";
    tickerPrice.second = 0.00141321;
    Global::TickerPrice::set(tickerPrice);
    tickerPrice.first = "Tester/Coin";
    tickerPrice.second = 0.1;
    Global::TickerPrice::set(tickerPrice);
    tickerPrice.first = "USDT";
    tickerPrice.second = 1;
    Global::TickerPrice::set(tickerPrice);

    loopTimer.setInterval(250);
    connect(&loopTimer, &QTimer::timeout, this, &MainWindow::on_timerLoopTick);
    loopTimer.start();

    ui->walletNetworkNameLabel->setParent(this);
    Style::setLabelStyle(ui->walletNetworkNameLabel);
    ui->walletNetworkNameLabel->setGeometry(ui->walletNetworkNameLabel->geometry().x(),
                                            ui->walletNetworkNameLabel->geometry().y() + Global::Layout::getHeaderHeight(),
                                            ui->walletNetworkNameLabel->geometry().width(),
                                            ui->walletNetworkNameLabel->geometry().height());
    ui->walletNetworkNameLabel->clear();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setScale() {
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    this->setStyleSheet("background: " + Color::getBackground());
    Style::setMainTabStyle(ui->mainTabWidget);
    ui->walletNetworkNameLabel->setGeometry(Global::Layout::scaleRect(walletNetworkNameLabelQRectBack));
    ui->walletNetworkNameLabel->setFont(Global::Layout::scaleFontOffset(walletNetworkNameLabelQFontBack));
    ui->mainTabWidget->setGeometry(Global::Layout::getLayoutGeometryScaled());
    ui->mainTabWidget->setFont(Global::Layout::scaleFontOffset(ui->mainTabWidget->font()));
}

void MainWindow::setPage(int index) {
    lastIndex = index;
}

void MainWindow::on_mainTabWidget_currentChanged(int index) {
    if(!initialized)
        return;
    qDebug() << "selected" << index;
    if(lastIndex != index) {
        switch(lastIndex) {
        case Global::Page::STAKING:
            pageStaking->close();
            break;
        case Global::Page::SWAP:
            pageSwap->close();
            break;
        case Global::Page::ACCOUNT:
            pageAccount->close();
            break;
        case Global::Page::DEX:
            pageDex->close();
            break;
        case Global::Page::MORE:
            pageMore->close();
            break;
        default:
            break;
        }
        switch(index) {
        case Global::Page::STAKING:
            pageStaking->open();
            break;
        case Global::Page::SWAP:
            pageSwap->open();
            break;
        case Global::Page::ACCOUNT:
            pageAccount->open();
            break;
        case Global::Page::DEX:
            pageDex->open();
            break;
        case Global::Page::MORE:
            pageMore->open();
            break;
        default:
            break;
        }
        lastIndex = index;
    }
}

void MainWindow::on_timerLoopTick() {
    switch(lastIndex) {
    case Global::Page::STAKING:
        pageStaking->loop();
        break;
    case Global::Page::SWAP:
        pageSwap->loop();
        break;
    case Global::Page::ACCOUNT:
        pageAccount->loop();
        break;
    case Global::Page::DEX:
        pageDex->loop();
        break;
    case Global::Page::MORE:
        pageMore->loop();
        break;
    case Global::Page::HISTORY:
        pageHistory->loop();
        break;
    case Global::Page::SEND:
        pageSend->loop();
        break;
    case Global::Page::RECEIVE:
        pageReceive->loop();
        break;
    default:
        break;
    }
    if(accountNetworkName.compare(Global::Util::getAccountNameAndNetwork())) {
        accountNetworkName = Global::Util::getAccountNameAndNetwork();
        ui->walletNetworkNameLabel->setText(Global::Util::getAccountNameAndNetwork());
    }
}

void MainWindow::switchTranslator(const QString filename) {
    // remove the old translator
    if(qtTranslator != NULL)
        qApp->removeTranslator(qtTranslator);
    qtTranslator = new QTranslator(this);
    // load the new translator
    //QString path = QApplication::applicationDirPath();
    //path.append("/languages/");
    if(qtTranslator->load(":/languages/languages/LyraPcWalletManager_" + filename + ".qm")) //Here Path and Filename has to be entered because the system didn't find the QM Files else
        qApp->installTranslator(qtTranslator);
    else
        qDebug() << "Unable to find translation file";
}

