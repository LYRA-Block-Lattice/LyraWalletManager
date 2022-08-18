#include "pagestaking.h"
#include "ui_pagestaking.h"

#include <QStandardItemModel>
#include <QDateTime>
#include <QGraphicsOpacityEffect>
#include <QInputDialog>

#include "global.h"
#include "color.h"
#include "style.h"

#include "translate/translate.h"

PageStaking::PageStaking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageStaking) {
    ui->setupUi(this);
    Parent = parent;

    ui->statusLabel->setVisible(false);

    StakingList = new PageStakingList(this);

    progressMovie = new QMovie(":/res/ic/res/ic/waiting_light.gif");
    if (progressMovie->isValid()) {
        progressLabel = new QLabel(this);
        progressLabel->setGeometry(135,230,40,40);
        progressLabel->setScaledContents(true);
        progressLabel->setMovie(progressMovie);
        progressLabel->setVisible(false);
        //progressMovie->start();
    }

    ui->statusLabel->setParent(StakingList);

    // Backup items geometry, size and font.
    HeaderFrameQRectBack = ui->headerFrame->geometry();
    TitleLabelQRectBack = ui->titleLabel->geometry();
    TitleLabelQFontBack = ui->titleLabel->font();

    StakePushButtonQRectBack = ui->stakePushButton->geometry();
    StakePushButtonQFontBack = ui->stakePushButton->font();
    UnstakePushButtonQRectBack = ui->unstakePushButton->geometry();
    UnstakePushButtonQFontBack = ui->unstakePushButton->font();
    AddStakingAccountPushButtonQRectBack = ui->addStakingAccountPushButton->geometry();
    AddStakingAccountPushButtonQFontBack = ui->addStakingAccountPushButton->font();
    ProgressMovieQRectBack = progressLabel->geometry();

    statusLabelQRectBack = ui->statusLabel->geometry();
    statusLabelQFontBack = ui->statusLabel->font();

    setStyle();

    StakingList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    StakingList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    StakingList->scrollToTop();
    StakingList->setEnabled(true);

    setScale();

    connect(StakingList, &PageStakingList::itemClicked, this, [=](int item) {
        qDebug() << "Selected item: " << item;
    });

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

    connect(&delayGetBrokerAccounts, &QTimer::timeout, this, [=] {
        delayGetBrokerAccounts.stop();
        fetchGetBrokerAccounts();
    });
}

PageStaking::~PageStaking() {
    delete ui;
}

void PageStaking::open() {
    StakingList->clearAll();
    fetchGetBrokerAccounts();
}

void PageStaking::close() {

}

void PageStaking::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(HeaderFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(TitleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(TitleLabelQFontBack));

    StakingList->setGeometry(QRect(0,
                                   Global::Layout::scaleValueX(61),
                                   Parent->geometry().width(),
                                   Global::Layout::scaleValueX(410 - 61)));

    ui->stakePushButton->setGeometry(Global::Layout::scaleRect(StakePushButtonQRectBack));
    ui->stakePushButton->setFont(Global::Layout::scaleFontOffset(StakePushButtonQFontBack));
    ui->unstakePushButton->setGeometry(Global::Layout::scaleRect(UnstakePushButtonQRectBack));
    ui->unstakePushButton->setFont(Global::Layout::scaleFontOffset(UnstakePushButtonQFontBack));
    ui->addStakingAccountPushButton->setGeometry(Global::Layout::scaleRect(AddStakingAccountPushButtonQRectBack));
    ui->addStakingAccountPushButton->setFont(Global::Layout::scaleFontOffset(AddStakingAccountPushButtonQFontBack));
    progressLabel->setGeometry(Global::Layout::scaleRect(ProgressMovieQRectBack));

    ui->statusLabel->setGeometry(Global::Layout::scaleRect(statusLabelQRectBack));;
    ui->statusLabel->setFont(Global::Layout::scaleFontOffset(statusLabelQFontBack));
}

void PageStaking::setStyle() {
    Style::setButtonDefaultStyle(ui->stakePushButton);
    Style::setButtonDefaultStyle(ui->addStakingAccountPushButton);
    Style::setButtonDefaultStyle(ui->unstakePushButton);
}

void PageStaking::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
    }
}

void PageStaking::fetchGetBrokerAccounts() {
    progressLabel->setVisible(true);
    progressMovie->start();
    getBrokerAccountsThread = new WalletRpc::GetBrokerAccounts;
    getBrokerAccountsWorkerThread = new QThread;
    getBrokerAccountsThread->moveToThread(getBrokerAccountsWorkerThread);
    connect(getBrokerAccountsWorkerThread, &QThread::finished, getBrokerAccountsThread, &QObject::deleteLater);
    connect(this, &PageStaking::getBrokerAccountsStartFetch, getBrokerAccountsThread, &WalletRpc::GetBrokerAccounts::doWork);
    connect(getBrokerAccountsThread, &WalletRpc::GetBrokerAccounts::resultReady, this, [=](const QString d) {
        StakingList->clearAll();
        RpcClass::GetBrokerAccounts getBrokerAccountsInst = RpcClass::GetBrokerAccounts(d);
        if(getBrokerAccountsInst.getValid()) {
            QDateTime now = QDateTime::currentDateTime();
            int64_t timeNow = now.currentDateTimeUtc().toMSecsSinceEpoch();
            QList<RpcClass::GetBrokerAccounts::stakingsEntry_t> entrys = getBrokerAccountsInst.getStakings();
            foreach(RpcClass::GetBrokerAccounts::stakingsEntry_t entry, entrys) {
                PageStakingList::item_t item;
                item.accountName = entry.name;
                item.stakingAccountId = entry.stkid;
                item.profitingAccountId = entry.voting;
                if(entry.start.compare("9999-12-31T23:59:59.9999999Z")) {
                    QDateTime dT = QDateTime::fromString(entry.start, Qt::ISODateWithMs);
                    QDateTime date = dT.addDays(entry.days);
                    if(date.toMSecsSinceEpoch() < timeNow) {
                        item.expiryDate = "?";
                        item.status = false;
                    } else {
                        item.expiryDate = date.toString("yyyy/MM/dd hh:mm:ss");
                        item.status = true;
                    }
                } else {
                    item.expiryDate = "?";
                    item.status = false;
                }
                item.accountValidity = QString::asprintf("%d Days", entry.days);
                item.amount = QString::asprintf("%s LYR", Global::Util::normaliseNumber(entry.amount).toUtf8().data());
                StakingList->addMessage(item);
            }
            StakingList->scrollToTop();
        }
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    connect(getBrokerAccountsThread, &WalletRpc::GetBrokerAccounts::resultError, this, [=] {
        StakingList->clearAll();
        progressLabel->setVisible(false);
        progressMovie->stop();
    });
    getBrokerAccountsWorkerThread->start();
    emit getBrokerAccountsStartFetch();
}

void PageStaking::startAddStaking(double amount) {
    QModelIndex index = StakingList->currentIndex();
    if(index.row() < 0) {
        ui->statusLabel->setText(Tr("No staking account selected"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return;
    }
    ui->statusLabel->setText(Tr("Sending stake"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    addStakingThread = new WalletRpc::AddStaking;
    addStakingsWorkerThread = new QThread;
    addStakingThread->moveToThread(addStakingsWorkerThread);
    connect(addStakingsWorkerThread, &QThread::finished, addStakingThread, &QObject::deleteLater);
    connect(this, &PageStaking::addStakingStartFetch, addStakingThread, &WalletRpc::AddStaking::doWork);
    connect(addStakingThread, &WalletRpc::AddStaking::resultReady, this, [=](const QString d) {
        RpcClass::AddStaking addStakingInst = RpcClass::AddStaking(d);
        if(addStakingInst.getValid()) {
            ui->statusLabel->setText(Tr("Add stake succesfull"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            delayGetBrokerAccounts.setInterval(5000);
            delayGetBrokerAccounts.start();
            progressLabel->setVisible(true);
            progressMovie->start();
        } else {
            ui->statusLabel->setText(addStakingInst.getMessage());
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
        }
    });
    connect(addStakingThread, &WalletRpc::AddStaking::resultError, this, [=] {
        ui->statusLabel->setText(Tr("Error staking"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    });
    addStakingsWorkerThread->start();
    emit addStakingStartFetch(StakingList->getStkId(index.row()), amount);
}

void PageStaking::startUnstake() {

}

void PageStaking::on_stakePushButton_clicked() {
    QModelIndex index = StakingList->currentIndex();
    if(index.row() < 0) {
        ui->statusLabel->setText(Tr("No staking account selected"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return;
    }
    QString sendreview = Tr("How much do you want to stake") + "\n" +
            Tr("to accont") + ":\n" +
            StakingList->getName(index.row());
    QInputDialog *inputDialog = new QInputDialog(this);
    inputDialog->setInputMode(QInputDialog::TextInput);
    inputDialog->setWindowTitle(Tr("Stake"));
    inputDialog->setLabelText(sendreview);
    QLineEdit *lineEdit = inputDialog->findChild<QLineEdit *>();
    lineEdit->setPlaceholderText("Amount");
    lineEdit->setStyleSheet(
                            "QLineEdit[text=\"\"]{"
                            "    color:gray;"
                            "}"
                            "    QLineEdit {"
                            "    color:black;"
                            "    border-style: solid;"
                            "    border-radius: 5px;"
                            "    border-width: 1px;"
                            "    border-color: lightgrey;"
                            "    padding-left: 4px;"
                            "    padding-right: 4px;"
                            "    height: 31px;"
                            "}");

    connect(inputDialog, &QInputDialog::textValueSelected, this, [=](QString text) {
        startAddStaking(text.remove(',').toDouble());
    });
    connect(lineEdit, &QLineEdit::textChanged, this, [=](QString text) {
        Q_UNUSED(text)
        lineEdit->style()->polish(lineEdit);
        QString s = text;
        bool validate;
        s.remove(',').toDouble(&validate);
        if(s.length() > 0) {
            if(!validate) {
                lineEdit->setText(Global::Util::normaliseNumber(s.left(s.length() - 1), false));
            } else {
                lineEdit->setText(Global::Util::normaliseNumber(s.remove(','), false));
            }
        }
    });
    inputDialog->open();
}

void PageStaking::on_unstakePushButton_clicked() {
    QModelIndex index = StakingList->currentIndex();
    if(index.row() < 0) {
        ui->statusLabel->setText(Tr("No staking account selected"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return;
    }
    QString sendreview = Tr("Do you want to unstake") + "\n" +
            Tr("from account") + ":\n" +
            StakingList->getName(index.row());
    QMessageBox* const message = new QMessageBox(QMessageBox::Icon::Question, tr("Un Stake") + "?",
        sendreview, QMessageBox::Button::Yes | QMessageBox::Button::No, this);
    message->setDefaultButton(QMessageBox::Button::No);
    message->open();
    connect(message, &QDialog::finished, this, [message, this] {
        message->deleteLater();
        QModelIndex index = StakingList->currentIndex();
        if (message->result() == QMessageBox::Button::Yes) {
            ui->statusLabel->setText(Tr("Un Stacking"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            unStakingThread = new WalletRpc::UnStaking;
            unStakingsWorkerThread = new QThread;
            unStakingThread->moveToThread(unStakingsWorkerThread);
            connect(unStakingsWorkerThread, &QThread::finished, unStakingThread, &QObject::deleteLater);
            connect(this, &PageStaking::unStakingStartFetch, unStakingThread, &WalletRpc::UnStaking::doWork);
            connect(unStakingThread, &WalletRpc::UnStaking::resultReady, this, [=](const QString d) {
                RpcClass::UnStaking unStakingInst = RpcClass::UnStaking(d);
                if(unStakingInst.getValid()) {
                    ui->statusLabel->setText(Tr("Un Stake succesfull"));
                    fadeCount = FADE_COUNT_START_VALE;
                    fadeTimer.start();
                    delayGetBrokerAccounts.setInterval(5000);
                    delayGetBrokerAccounts.start();
                    progressLabel->setVisible(true);
                    progressMovie->start();
                } else {
                    ui->statusLabel->setText(unStakingInst.getMessage());
                    fadeCount = FADE_COUNT_START_VALE;
                    fadeTimer.start();
                }
            });
            connect(unStakingThread, &WalletRpc::UnStaking::resultError, this, [=] {
                ui->statusLabel->setText(Tr("Error un staking"));
                fadeCount = FADE_COUNT_START_VALE;
                fadeTimer.start();
            });
            unStakingsWorkerThread->start();
            emit unStakingStartFetch(StakingList->getStkId(index.row()));
        }
    });
}

void PageStaking::on_addStakingAccountPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::ADD_STAKING_ACCOUNT);
}

