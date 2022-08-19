#include "pageaddstakingaccount.h"
#include "ui_pageaddstakingaccount.h"

#include <QGraphicsOpacityEffect>

#include "style.h"
#include "global.h"
#include "globallyra.h"

#include "translate/translate.h"
#include "wallet/history.h"

static QTimer fadeTimer;
static int fadeCount;

PageAddStakingAccount::PageAddStakingAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageAddStakingAccount) {
    ui->setupUi(this);

    StakingList = new PageStakingAddAccountList(ui->stakeForGroupBox);

    progressMovie = new QMovie(":/res/ic/res/ic/waiting_light.gif");
    if (progressMovie->isValid()) {
        progressLabel = new QLabel(this);
        progressLabel->setGeometry(135,230,40,40);
        progressLabel->setScaledContents(true);
        progressLabel->setMovie(progressMovie);
        progressLabel->setVisible(false);
        //progressMovie->start();
    }
    // Backup items geometry, size and font.
    //ui->headerFrame->setParent(this);
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();

    AmountGroupBoxQRectBack = ui->amountGroupBox->geometry();
    AmountLineEditQRectBack = ui->amountLineEdit->geometry();
    AmountLineEditQFontBack = ui->amountLineEdit->font();
    MaxPushButtonQRectBack = ui->maxPushButton->geometry();
    MaxPushButtonQSizeBack = ui->maxPushButton->iconSize();
    AvailablePushButtonQRectBack = ui->availablePushButton->geometry();
    AvailablePushButtonQFontBack = ui->availablePushButton->font();

    StakeForGroupBoxQRectBack = ui->stakeForGroupBox->geometry();

    NameDaysGroupBoxQRectBack = ui->nameDaysGroupBox->geometry();
    NameLineEditQRectBack = ui->nameLineEdit->geometry();
    NameLineEditQFontBack = ui->nameLineEdit->font();
    DaysLineEditQRectBack = ui->daysLineEdit->geometry();
    DaysLineEditQFontBack = ui->daysLineEdit->font();
    PreviewPushButtonQRectBack = ui->previewPushButton->geometry();
    PreviewPushButtonQFontBack = ui->previewPushButton->font();

    ProgressMovieQRectBack = progressLabel->geometry();

    statusLabelQRectBack = ui->statusLabel->geometry();
    statusLabelQFontBack = ui->statusLabel->font();

    setStyle();

    StakingList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    StakingList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    StakingList->scrollToTop();
    StakingList->setEnabled(true);
    StakingList->setStyleSheet("border-color:transparent;");

    setScale();

    connect(StakingList, &PageStakingAddAccountList::itemClicked, this, [=](int index) {
        findAllStakings(index);
    });

    ui->statusLabel->setParent(this);
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
    connect(&delayGoStakingPage, &QTimer::timeout, this, [=] {
        delayGoStakingPage.stop();
        progressLabel->setVisible(false);
        progressMovie->stop();
        Global::Page::goManagerPage(Global::Page::STAKING);
    });
}


PageAddStakingAccount::~PageAddStakingAccount() {
    delete ui;
}

void PageAddStakingAccount::open() {
    ui->amountLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->daysLineEdit->clear();
    double balance = Wallet::History::getTickerBalance("LYR");
    ui->availablePushButton->setText(QString::asprintf("%s: %s LYR", Tr("Available").toUtf8().data(), Global::Util::normaliseNumber(balance).toUtf8().data()));
    findAllProfitingAccounts();
}

void PageAddStakingAccount::close() {

}

void PageAddStakingAccount::setScale() {
    setGeometry(Global::Layout::getLayoutGeometryScaled());
    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->amountGroupBox->setGeometry(Global::Layout::scaleRect(AmountGroupBoxQRectBack));
    ui->amountLineEdit->setGeometry(Global::Layout::scaleRect(AmountLineEditQRectBack));
    ui->amountLineEdit->setFont(Global::Layout::scaleFontOffset(AmountLineEditQFontBack));
    ui->maxPushButton->setGeometry(Global::Layout::scaleRect(MaxPushButtonQRectBack));
    ui->maxPushButton->setIconSize(Global::Layout::scaleSize(MaxPushButtonQSizeBack));
    ui->availablePushButton->setGeometry(Global::Layout::scaleRect(AvailablePushButtonQRectBack));
    ui->availablePushButton->setFont(Global::Layout::scaleFontOffset(AvailablePushButtonQFontBack));

    ui->stakeForGroupBox->setGeometry(Global::Layout::scaleRect(StakeForGroupBoxQRectBack));

    ui->nameDaysGroupBox->setGeometry(Global::Layout::scaleRect(NameDaysGroupBoxQRectBack));
    ui->nameLineEdit->setGeometry(Global::Layout::scaleRect(NameLineEditQRectBack));
    ui->nameLineEdit->setFont(Global::Layout::scaleFontOffset(NameLineEditQFontBack));
    ui->daysLineEdit->setGeometry(Global::Layout::scaleRect(DaysLineEditQRectBack));
    ui->daysLineEdit->setFont(Global::Layout::scaleFontOffset(DaysLineEditQFontBack));
    ui->previewPushButton->setGeometry(Global::Layout::scaleRect(PreviewPushButtonQRectBack));
    ui->previewPushButton->setFont(Global::Layout::scaleFontOffset(PreviewPushButtonQFontBack));

    QRect rect = ui->stakeForGroupBox->geometry();
    rect.moveTopLeft(QPoint(0,0));
    StakingList->setGeometry(rect);

    progressLabel->setGeometry(Global::Layout::scaleRect(ProgressMovieQRectBack));

    ui->statusLabel->setGeometry(Global::Layout::scaleRect(statusLabelQRectBack));;
    ui->statusLabel->setFont(Global::Layout::scaleFontOffset(statusLabelQFontBack));
}

void PageAddStakingAccount::setStyle() {
    Style::setLineEditlWhiteStyle(ui->amountLineEdit);
    Style::setLineEditlWhiteStyle(ui->nameLineEdit);
    Style::setLineEditlWhiteStyle(ui->daysLineEdit);
    Style::setButtontOnHeaderStyle(ui->backPushButton);
    Style::setButtonDefaultStyle(ui->previewPushButton);
    Style::setGroupBoxStyle(ui->amountGroupBox);
    Style::setGroupBoxStyle(ui->stakeForGroupBox);
    Style::setGroupBoxStyle(ui->nameDaysGroupBox);
    Style::setButtonTransparentStyle(ui->maxPushButton);
}

void PageAddStakingAccount::loop() {

}

void PageAddStakingAccount::findAllProfitingAccounts() {
    progressLabel->setVisible(true);
    progressMovie->start();
    StakingList->clearAll();
    allProfitingAccountsFetchWorker = new WebGet;
    allProfitingAccountsFetchWorkerThread = new QThread;
    allProfitingAccountsFetchWorker->moveToThread(allProfitingAccountsFetchWorkerThread);
    connect(allProfitingAccountsFetchWorker, &WebGet::resultReady, this, [=](QString s) {
        WebClass::AllProfitingAccounts allProfitingAccountsInst = WebClass::AllProfitingAccounts(s);
        if(allProfitingAccountsInst.getValid()) {
            WebClass::AllProfitingAccounts::entry_t entry;
            foreach(entry, allProfitingAccountsInst.getAccountList()) {
                PageStakingAddAccountList::item_t item;
                item.accountName = entry.Name;
                item.accountType = entry.PType;
                item.shareRatio = QString::asprintf("%s%%", Global::Util::normaliseNumber(entry.ShareRatio * 100).toUtf8().data());
                item.seats = QString::number(entry.Seats);
                QDateTime dT = QDateTime::fromString(entry.TimeStamp, Qt::ISODateWithMs);
                item.timeStamp = dT.toString("yyyy/MM/dd hh:mm:ss");
                item.totalProfit = QString::asprintf("%s LYR", Global::Util::normaliseNumber(entry.TotalProfit).toUtf8().data());
                item.totalStaked = "0 LYR";
                item.yourShare = "none";
                item.fetch = false;
                item.pftId = entry.AccountId;
                StakingList->addMessage(item);
            }
            StakingList->scrollToTop();
            progressLabel->setVisible(false);
            progressMovie->stop();
        }
    });
    connect(allProfitingAccountsFetchWorker, &WebGet::resultError, this, [=](QString s) {
        Q_UNUSED(s)
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    connect(allProfitingAccountsFetchWorkerThread, &QThread::finished, allProfitingAccountsFetchWorker, &QObject::deleteLater);
    connect(this, &PageAddStakingAccount::allProfitingAccountsFetch, allProfitingAccountsFetchWorker, &WebGet::doWork);
    allProfitingAccountsFetchWorkerThread->start();
    emit allProfitingAccountsFetch("https://" + Global::Network::getNodeAddress().second + LYRA_NODE_API_URL +
                                 "/FindAllProfitingAccounts/?timeBeginTicks=0&timeEndTicks=3155378975999999999");

}

void PageAddStakingAccount::findAllStakings(int index) {
    if(index < 0) {
        ui->statusLabel->setText(Tr("No profiting account selected"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return;
    }
    progressLabel->setVisible(true);
    progressMovie->start();
    allStakingsFetchWorker = new WebGet;
    allStakingsFetchWorkerThread = new QThread;
    allStakingsFetchWorker->moveToThread(allStakingsFetchWorkerThread);
    connect(allStakingsFetchWorker, &WebGet::resultReady, this, [=](QString s) {
        WebClass::AllStakings allStakingsInst = WebClass::AllStakings(s);
        if(allStakingsInst.getValid()) {
            progressLabel->setVisible(false);
            progressMovie->stop();

            double amount = 0.0;
            WebClass::AllStakings::entry_t entry;
            foreach( entry, allStakingsInst.getStakingList()) {
                amount += entry.Amount;
            }

            StakingList->setFetch(index, amount,
                                  (ui->amountLineEdit->text().remove(',').toDouble() /
                                   ((StakingList->getTotalStaked(index) + ui->amountLineEdit->text().remove(',').toDouble())) *
                                  StakingList->getShareRatio(index)) * 100);
        }
    });
    connect(allStakingsFetchWorker, &WebGet::resultError, this, [=](QString s) {
        Q_UNUSED(s)
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    connect(allStakingsFetchWorkerThread, &QThread::finished, allStakingsFetchWorker, &QObject::deleteLater);
    connect(this, &PageAddStakingAccount::allStakingsFetch, allStakingsFetchWorker, &WebGet::doWork);
    allStakingsFetchWorkerThread->start();
    emit allStakingsFetch("https://" + Global::Network::getNodeAddress().second + LYRA_NODE_API_URL +
                          "/FindAllStakings/?pftid=" + StakingList->getPftId(index) + "&timeBeforeTicks=637793186320590000");

}

void PageAddStakingAccount::createStakingAccount() {
    QModelIndex index = StakingList->currentIndex();
    if(index.row() < 0) {
        ui->statusLabel->setText(Tr("No profiting account selected"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return;
    }
    ui->statusLabel->setText(Tr("Start creating stake account"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    createStakingAccountThread = new WalletRpc::CreateStakingAccount;
    createStakingAccountWorkerThread = new QThread;
    createStakingAccountThread->moveToThread(createStakingAccountWorkerThread);
    connect(createStakingAccountWorkerThread, &QThread::finished, createStakingAccountThread, &QObject::deleteLater);
    connect(this, &PageAddStakingAccount::createStakingAccountStart, createStakingAccountThread, &WalletRpc::CreateStakingAccount::doWork);
    connect(createStakingAccountThread, &WalletRpc::CreateStakingAccount::resultReady, this, [=](const QString d) {
        RpcClass::CreateStakingAccount createStakingAccountInst = RpcClass::CreateStakingAccount(d);
        if(createStakingAccountInst.getValid()) {
            startAddStaking(createStakingAccountInst.getStkid());
        }
    });
    connect(createStakingAccountThread, &WalletRpc::CreateStakingAccount::resultError, this, [=](QString message) {
        ui->statusLabel->setText(message);
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    createStakingAccountWorkerThread->start();
    emit createStakingAccountStart(ui->nameLineEdit->text(),
                                   StakingList->getPftId(index.row()) ,
                                   ui->daysLineEdit->text().toInt());
}

void PageAddStakingAccount::startAddStaking(QString stkId) {
    ui->statusLabel->setText(Tr("Sending stake"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    addStakingThread = new WalletRpc::AddStaking;
    addStakingsWorkerThread = new QThread;
    addStakingThread->moveToThread(addStakingsWorkerThread);
    connect(addStakingsWorkerThread, &QThread::finished, addStakingThread, &QObject::deleteLater);
    connect(this, &PageAddStakingAccount::addStakingStart, addStakingThread, &WalletRpc::AddStaking::doWork);
    connect(addStakingThread, &WalletRpc::AddStaking::resultReady, this, [=](const QString d) {
        RpcClass::AddStaking addStakingInst = RpcClass::AddStaking(d);
        if(addStakingInst.getValid()) {
            ui->statusLabel->setText(Tr("Add stake succesfull"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            delayGoStakingPage.setInterval(5000);
            delayGoStakingPage.start();
            progressLabel->setVisible(true);
            progressMovie->start();
        } else {
            ui->statusLabel->setText(addStakingInst.getMessage());
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
        }
    });
    connect(addStakingThread, &WalletRpc::AddStaking::resultError, this, [=](QString message) {
        ui->statusLabel->setText(message);
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    addStakingsWorkerThread->start();
    emit addStakingStart(stkId, ui->amountLineEdit->text().remove(',').toDouble());
}
void PageAddStakingAccount::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::STAKING);
}

void PageAddStakingAccount::on_amountLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->amountLineEdit->style()->polish(ui->amountLineEdit);
    QString s = arg1;
    bool validate;
    s.remove(',').toDouble(&validate);
    if(s.length() > 0) {
        if(!validate) {
            ui->amountLineEdit->setText(Global::Util::normaliseNumber(s.left(s.length() - 1), false));
        } else {
            ui->amountLineEdit->setText(Global::Util::normaliseNumber(s.remove(','), false));
        }
    }
}

void PageAddStakingAccount::on_maxPushButton_clicked() {

}

void PageAddStakingAccount::on_nameLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->nameLineEdit->style()->polish(ui->nameLineEdit);
    if(ui->nameLineEdit->text().length() > 3)
        Style::setLineEditlWhiteStyle(ui->nameLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->nameLineEdit, "red");
}

void PageAddStakingAccount::on_daysLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->daysLineEdit->style()->polish(ui->daysLineEdit);
    bool valid;
    int d = ui->daysLineEdit->text().toInt(&valid);
    if(d >= 3 && d < 365000 && valid)
        Style::setLineEditlWhiteStyle(ui->daysLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->daysLineEdit, "red");
}

void PageAddStakingAccount::on_previewPushButton_clicked() {
    QString sendreview = Tr("You will create") + ":\n" +
            ui->nameLineEdit->text() + ":\n" +
            "and stake:\n" +
            ui->amountLineEdit->text() + ":\n" +
            "for:\n" +
            ui->daysLineEdit->text() + "Days";
    QMessageBox* const message = new QMessageBox(QMessageBox::Icon::Question, tr("Send") + "?",
        sendreview, QMessageBox::Button::Yes | QMessageBox::Button::No, this);
    message->setDefaultButton(QMessageBox::Button::No);
    message->open();
    connect(message, &QDialog::finished, this, [message, this] {
        message->deleteLater();
        if (message->result() == QMessageBox::Button::Yes) {
            createStakingAccount();
        }
    });
}

