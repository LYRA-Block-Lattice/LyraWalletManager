#include "pagetransactiondetail.h"
#include "ui_pagetransactiondetail.h"

#include <QClipboard>
#include <QGraphicsOpacityEffect>
#include <QDesktopServices>
#include <QUrl>

#include "style.h"
#include "global.h"

PageTransactionDetail::PageTransactionDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageTransactionDetail) {
    ui->setupUi(this);
    ui->copiedLabel->setVisible(false);
    setStyle();

    ui->amountInAccountValueLabel->setGeometry(QRect(10, 390, 251, Global::Layout::getLayoutGeometry().height() - 390 - 10));
    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    lyraLogoLabelQRectBack = ui->lyraLogoLabel->geometry();
    //lyraLogoLabelQSizeBack = ui->lyraLogoLabel->iconSize();
    label1QRectBack = ui->titleLabel->geometry();
    label1QFontBack = ui->titleLabel->font();

    heightLabelQRectBack = ui->heightLabel->geometry();
    heightLabelQFontBack = ui->heightLabel->font();
    directionLabelQRectBack = ui->directionLabel->geometry();
    directionLabelQFontBack = ui->directionLabel->font();
    timeStampLabelQRectBack = ui->timeStampLabel->geometry();
    timeStampLabelQFontBack = ui->timeStampLabel->font();
    sendAccountIdLabelQRectBack = ui->sendAccountIdLabel->geometry();
    sendAccountIdLabelQFontBack = ui->sendAccountIdLabel->font();
    sendHashLabelQRectBack = ui->sendHashLabel->geometry();
    sendHashLabelQFontBack = ui->sendHashLabel->font();
    receiveAccountIdLabelQRectBack = ui->receiveAccountIdLabel->geometry();
    receiveAccountIdLabelQFontBack = ui->receiveAccountIdLabel->font();
    receiveHashLabelQRectBack = ui->receiveHashLabel->geometry();
    receiveHashLabelQFontBack = ui->receiveHashLabel->font();
    amountTransferredLabelQRectBack = ui->amountTransfrredLabel->geometry();
    amountTransferredLabelQFontBack = ui->amountTransfrredLabel->font();
    amountInAccountLabelQRectBack = ui->amountInAccountLabel->geometry();
    amountInAccountLabelQFontBack = ui->amountInAccountLabel->font();

    heightValueLabelQRectBack = ui->heightValueLabel->geometry();
    heightValueLabelQFontBack = ui->heightValueLabel->font();
    directionValueLabelQRectBack = ui->directionValueLabel->geometry();
    directionValueLabelQFontBack = ui->directionValueLabel->font();
    timeStampValueLabelQRectBack = ui->timeStampValueLabel->geometry();
    timeStampValueLabelQFontBack = ui->timeStampValueLabel->font();
    sendAccountIdValueLabelQRectBack = ui->sendAccountIdValueLabel->geometry();
    sendAccountIdValueLabelQFontBack = ui->sendAccountIdValueLabel->font();
    sendHashValueLabelQRectBack = ui->sendHashValueLabel->geometry();
    sendHashValueLabelQFontBack = ui->sendHashValueLabel->font();
    receiveAccountIdValueLabelQRectBack = ui->receiveAccountIdValueLabel->geometry();
    receiveAccountIdValueLabelQFontBack = ui->receiveAccountIdValueLabel->font();
    receiveHashValueLabelQRectBack = ui->receiveHashValueLabel->geometry();
    receiveHashValueLabelQFontBack = ui->receiveHashValueLabel->font();
    amountTransferredValueLabelQRectBack = ui->amountTransferedValueLabel->geometry();
    amountTransferredValueLabelQFontBack = ui->amountTransferedValueLabel->font();
    amountInAccountValueLabelQRectBack = ui->amountInAccountValueLabel->geometry();
    amountInAccountValueLabelQFontBack = ui->amountInAccountValueLabel->font();


    directionPushButtonQRectBack = ui->directionPushButton->geometry();
    directionPushButtonIconQSizeBack = ui->directionPushButton->iconSize();
    copyTimeStampPushButtonQRectBack = ui->copyTimeStampPushButton->geometry();
    copyTimeStampPushButtonIconQSizeBack = ui->copyTimeStampPushButton->iconSize();
    openSendAccountIdPushButtonQRectBack = ui->openSendAccountIdPushButton->geometry();
    openSendAccountIdPushButtonIconQSizeBack = ui->openSendAccountIdPushButton->iconSize();
    openSendHashPushButtonQRectBack = ui->openSendHashPushButton->geometry();
    openSendHashPushButtonIconQSizeBack = ui->openSendHashPushButton->iconSize();
    openReceiveAccountIdPushButtonQRectBack = ui->openReceiveAccountIdPushButton->geometry();
    openReceiveAccountIdPushButtonIconQSizeBack = ui->openReceiveAccountIdPushButton->iconSize();
    openReceiveHashPushButtonQRectBack = ui->openReceiveHashPushButton->geometry();
    openReceiveHashPushButtonIconQSizeBack = ui->openReceiveHashPushButton->iconSize();
    copyAmountTransferedPushButtonQRectBack = ui->copyAmountTransferedPushButton->geometry();
    copyAmountTransferedPushButtonIconQSizeBack = ui->copyAmountTransferedPushButton->iconSize();
    copyAmountInAccountPushButtonQRectBack = ui->copyAmountInAccountPushButton->geometry();
    copyAmountInAccountPushButtonIconQSizeBack = ui->copyAmountInAccountPushButton->iconSize();

    line_1LineQRectBack = ui->line_1->geometry();
    line_2LineQRectBack = ui->line_2->geometry();
    line_3LineQRectBack = ui->line_3->geometry();
    line_4LineQRectBack = ui->line_4->geometry();
    line_5LineQRectBack = ui->line_5->geometry();
    line_6LineQRectBack = ui->line_6->geometry();
    line_7LineQRectBack = ui->line_7->geometry();

    copiedLabelQRectBack = ui->copiedLabel->geometry();
    copiedLabelQFontBack = ui->copiedLabel->font();

    setScale();

    fadeTimer.setInterval(20);
    fadeTimer.stop();
    connect(&fadeTimer, &QTimer::timeout, this, &PageTransactionDetail::on_copiedFadeTimer);
}

PageTransactionDetail::~PageTransactionDetail() {
    delete ui;
}

void PageTransactionDetail::open(void *entry) {
    RpcClass::History::entry_t *Entry = (RpcClass::History::entry_t *)entry;
    this->entry = Entry;
    ui->lyraLogoLabel->setPixmap(QPixmap(Global::TickerIcon::get(Entry->Changes[0].first)));
    ui->heightValueLabel->setText(QString::number(Entry->Height));
    ui->directionValueLabel->setText(Entry->IsReceive ? "Receive" : "Send");
    ui->directionPushButton->setIcon(QPixmap(Entry->IsReceive ? ":/res/ic/res/ic/arrowLeftDown.png" : ":/res/ic/res/ic/arrowRightUp.png"));
    ui->timeStampValueLabel->setText(QDateTime::fromMSecsSinceEpoch(Entry->TimeStamp).toString("yyyy/MM/dd hh:mm:ss"));
    ui->sendAccountIdValueLabel->setText(Global::Util::truncateIdHash(Entry->SendAccountId, 17));
    ui->sendHashValueLabel->setText(Global::Util::truncateIdHash(Entry->SendHash, 17));
    ui->receiveAccountIdValueLabel->setText(Global::Util::truncateIdHash(Entry->RecvAccountId, 17));
    ui->receiveHashValueLabel->setText(Global::Util::truncateIdHash(Entry->RecvHash, 17));
    QString tmp;
    QPair<QString, double>tmpPair;
    foreach(tmpPair, Entry->Changes) {
        tmp.append(QString::asprintf("%s %s\n", Global::Util::normaliseNumber(tmpPair.second).toUtf8().data(), tmpPair.first.toUtf8().data()));
    }
    if(tmp.length() > 1)
        ui->amountTransferedValueLabel->setText(tmp.left(tmp.length() - 1));
    tmp.clear();
    foreach(tmpPair, Entry->Balances) {
        tmp.append(QString::asprintf("%s %s\n", Global::Util::normaliseNumber(tmpPair.second).toUtf8().data(), tmpPair.first.toUtf8().data()));
    }
    if(tmp.length() > 1)
        ui->amountInAccountValueLabel->setText(tmp.left(tmp.length() - 1));
}

void PageTransactionDetail::close() {

}

void PageTransactionDetail::setScale() {
    double scale = Global::Layout::labelFontScale();
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->lyraLogoLabel->setGeometry(Global::Layout::scaleRect(lyraLogoLabelQRectBack));
    //ui->lyraLogoLabel->setIconSize(Global::Layout::scaleSize(lyraLogoLabelQSizeBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(label1QRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(label1QFontBack));

    ui->heightLabel->setGeometry(Global::Layout::scaleRect(heightLabelQRectBack));;
    ui->heightLabel->setFont(Global::Layout::scaleFontOffset(heightLabelQFontBack, scale));;
    ui->directionLabel->setGeometry(Global::Layout::scaleRect(directionLabelQRectBack));;
    ui->directionLabel->setFont(Global::Layout::scaleFontOffset(directionLabelQFontBack, scale));
    ui->timeStampLabel->setGeometry(Global::Layout::scaleRect(timeStampLabelQRectBack));;
    ui->timeStampLabel->setFont(Global::Layout::scaleFontOffset(timeStampLabelQFontBack, scale));
    ui->sendAccountIdLabel->setGeometry(Global::Layout::scaleRect(sendAccountIdLabelQRectBack));;
    ui->sendAccountIdLabel->setFont(Global::Layout::scaleFontOffset(sendAccountIdLabelQFontBack, scale));
    ui->sendHashLabel->setGeometry(Global::Layout::scaleRect(sendHashLabelQRectBack));;
    ui->sendHashLabel->setFont(Global::Layout::scaleFontOffset(sendHashLabelQFontBack, scale));
    ui->receiveAccountIdLabel->setGeometry(Global::Layout::scaleRect(receiveAccountIdLabelQRectBack));;
    ui->receiveAccountIdLabel->setFont(Global::Layout::scaleFontOffset(receiveAccountIdLabelQFontBack, scale));
    ui->receiveHashLabel->setGeometry(Global::Layout::scaleRect(receiveHashLabelQRectBack));;
    ui->receiveHashLabel->setFont(Global::Layout::scaleFontOffset(receiveHashLabelQFontBack, scale));
    ui->amountTransfrredLabel->setGeometry(Global::Layout::scaleRect(amountTransferredLabelQRectBack));;
    ui->amountTransfrredLabel->setFont(Global::Layout::scaleFontOffset(amountTransferredLabelQFontBack, scale));
    ui->amountInAccountLabel->setGeometry(Global::Layout::scaleRect(amountInAccountLabelQRectBack));;
    ui->amountInAccountLabel->setFont(Global::Layout::scaleFontOffset(amountInAccountLabelQFontBack, scale));

    ui->heightValueLabel->setGeometry(Global::Layout::scaleRect(heightValueLabelQRectBack));;
    ui->heightValueLabel->setFont(Global::Layout::scaleFontOffset(heightValueLabelQFontBack, scale));
    ui->directionValueLabel->setGeometry(Global::Layout::scaleRect(directionValueLabelQRectBack));;
    ui->directionValueLabel->setFont(Global::Layout::scaleFontOffset(directionValueLabelQFontBack, scale));
    ui->timeStampValueLabel->setGeometry(Global::Layout::scaleRect(timeStampValueLabelQRectBack));;
    ui->timeStampValueLabel->setFont(Global::Layout::scaleFontOffset(timeStampValueLabelQFontBack, scale));
    ui->sendAccountIdValueLabel->setGeometry(Global::Layout::scaleRect(sendAccountIdValueLabelQRectBack));;
    ui->sendAccountIdValueLabel->setFont(Global::Layout::scaleFontOffset(sendAccountIdValueLabelQFontBack, scale));
    ui->sendHashValueLabel->setGeometry(Global::Layout::scaleRect(sendHashValueLabelQRectBack));;
    ui->sendHashValueLabel->setFont(Global::Layout::scaleFontOffset(sendHashValueLabelQFontBack, scale));
    ui->receiveAccountIdValueLabel->setGeometry(Global::Layout::scaleRect(receiveAccountIdValueLabelQRectBack));;
    ui->receiveAccountIdValueLabel->setFont(Global::Layout::scaleFontOffset(receiveAccountIdValueLabelQFontBack, scale));
    ui->receiveHashValueLabel->setGeometry(Global::Layout::scaleRect(receiveHashValueLabelQRectBack));;
    ui->receiveHashValueLabel->setFont(Global::Layout::scaleFontOffset(receiveHashValueLabelQFontBack, scale));
    ui->amountTransferedValueLabel->setGeometry(Global::Layout::scaleRect(amountTransferredValueLabelQRectBack));;
    ui->amountTransferedValueLabel->setFont(Global::Layout::scaleFontOffset(amountTransferredValueLabelQFontBack, scale));
    ui->amountInAccountValueLabel->setGeometry(Global::Layout::scaleRectXYNoDisplace(amountInAccountValueLabelQRectBack));;
    ui->amountInAccountValueLabel->setFont(Global::Layout::scaleFontOffset(amountInAccountValueLabelQFontBack, scale));

    ui->directionPushButton->setGeometry(Global::Layout::scaleRect(directionPushButtonQRectBack));;
    ui->directionPushButton->setIconSize(Global::Layout::scaleSize(directionPushButtonIconQSizeBack));
    ui->copyTimeStampPushButton->setGeometry(Global::Layout::scaleRect(copyTimeStampPushButtonQRectBack));;
    ui->copyTimeStampPushButton->setIconSize(Global::Layout::scaleSize(copyTimeStampPushButtonIconQSizeBack));
    ui->openSendAccountIdPushButton->setGeometry(Global::Layout::scaleRect(openSendAccountIdPushButtonQRectBack));;
    ui->openSendAccountIdPushButton->setIconSize(Global::Layout::scaleSize(openSendAccountIdPushButtonIconQSizeBack));
    ui->openSendHashPushButton->setGeometry(Global::Layout::scaleRect(openSendHashPushButtonQRectBack));;
    ui->openSendHashPushButton->setIconSize(Global::Layout::scaleSize(openSendHashPushButtonIconQSizeBack));
    ui->openReceiveAccountIdPushButton->setGeometry(Global::Layout::scaleRect(openReceiveAccountIdPushButtonQRectBack));;
    ui->openReceiveAccountIdPushButton->setIconSize(Global::Layout::scaleSize(openReceiveAccountIdPushButtonIconQSizeBack));
    ui->openReceiveHashPushButton->setGeometry(Global::Layout::scaleRect(openReceiveHashPushButtonQRectBack));;
    ui->openReceiveHashPushButton->setIconSize(Global::Layout::scaleSize(openReceiveHashPushButtonIconQSizeBack));
    ui->copyAmountTransferedPushButton->setGeometry(Global::Layout::scaleRect(copyAmountTransferedPushButtonQRectBack));;
    ui->copyAmountTransferedPushButton->setIconSize(Global::Layout::scaleSize(copyAmountTransferedPushButtonIconQSizeBack));
    ui->copyAmountInAccountPushButton->setGeometry(Global::Layout::scaleRect(copyAmountInAccountPushButtonQRectBack));;
    ui->copyAmountInAccountPushButton->setIconSize(Global::Layout::scaleSize(copyAmountInAccountPushButtonIconQSizeBack));

    ui->line_1->setGeometry(Global::Layout::scaleRect(line_1LineQRectBack));
    ui->line_2->setGeometry(Global::Layout::scaleRect(line_2LineQRectBack));
    ui->line_3->setGeometry(Global::Layout::scaleRect(line_3LineQRectBack));
    ui->line_4->setGeometry(Global::Layout::scaleRect(line_4LineQRectBack));
    ui->line_5->setGeometry(Global::Layout::scaleRect(line_5LineQRectBack));
    ui->line_6->setGeometry(Global::Layout::scaleRect(line_6LineQRectBack));
    ui->line_7->setGeometry(Global::Layout::scaleRect(line_7LineQRectBack));

    ui->copiedLabel->setGeometry(Global::Layout::scaleRect(copiedLabelQRectBack));;
    ui->copiedLabel->setFont(Global::Layout::scaleFontOffset(copiedLabelQFontBack, scale));
}

void PageTransactionDetail::setStyle() {
    Style::setLabelStyle(ui->titleLabel);

    Style::setButtonTransparentStyle(ui->backPushButton);
    Style::setButtonTransparentStyle(ui->copyTimeStampPushButton);
    Style::setButtonTransparentStyle(ui->openSendAccountIdPushButton);
    Style::setButtonTransparentStyle(ui->openSendHashPushButton);
    Style::setButtonTransparentStyle(ui->openReceiveAccountIdPushButton);
    Style::setButtonTransparentStyle(ui->openReceiveHashPushButton);
    Style::setButtonTransparentStyle(ui->copyAmountTransferedPushButton);
    Style::setButtonTransparentStyle(ui->copyAmountInAccountPushButton);
}

void PageTransactionDetail::loop() {

}

void PageTransactionDetail::on_copiedFadeTimer() {
    if(fadeCount == FADE_COUNT_START_VALE)
        ui->copiedLabel->setVisible(true);
    double opacity = 0;
    if(fadeCount == 0) {
        opacity = 0.0;
        ui->copiedLabel->setVisible(false);
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
    ui->copiedLabel->setGraphicsEffect(effect);
}

void PageTransactionDetail::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::HISTORY);
    close();
}

void PageTransactionDetail::on_copyTimeStampPushButton_clicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->timeStampValueLabel->text());
    QRect rect = ui->copiedLabel->geometry();
    int height = ui->copyTimeStampPushButton->geometry().height();
    rect.setY(ui->copyTimeStampPushButton->geometry().y());
    rect.setHeight(height);
    ui->copiedLabel->setGeometry(rect);
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

void PageTransactionDetail::on_openSendAccountIdPushButton_clicked() {
    QDesktopServices::openUrl(QUrl(Global::Network::getNebulaAddress() + "/showblock/" + entry->SendAccountId));
}

void PageTransactionDetail::on_openSendHashPushButton_clicked() {
    QDesktopServices::openUrl(QUrl(Global::Network::getNebulaAddress() + "/showblock/" + entry->SendHash));
}

void PageTransactionDetail::on_openReceiveAccountIdPushButton_clicked() {
    QDesktopServices::openUrl(QUrl(Global::Network::getNebulaAddress() + "/showblock/" + entry->RecvAccountId));
}

void PageTransactionDetail::on_openReceiveHashPushButton_clicked() {
    QDesktopServices::openUrl(QUrl(Global::Network::getNebulaAddress() + "/showblock/" + entry->RecvHash));
}

void PageTransactionDetail::on_copyAmountTransferedPushButton_clicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->amountTransferedValueLabel->text());
    QRect rect = ui->copiedLabel->geometry();
    int height = ui->copyTimeStampPushButton->geometry().height();
    rect.setY(ui->copyAmountTransferedPushButton->geometry().y());
    rect.setHeight(height);
    ui->copiedLabel->setGeometry(rect);
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

void PageTransactionDetail::on_copyAmountInAccountPushButton_clicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->amountInAccountValueLabel->text());
    QRect rect = ui->copiedLabel->geometry();
    int height = ui->copyTimeStampPushButton->geometry().height();
    rect.setY(ui->copyAmountInAccountPushButton->geometry().y());
    rect.setHeight(height);
    ui->copiedLabel->setGeometry(rect);
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

