#include "pagedexdeposit.h"
#include "ui_pagedexdeposit.h"

#include <QGraphicsOpacityEffect>
#include <QClipboard>

#include "global.h"
#include "color.h"
#include "style.h"

#include "translate/translate.h"


PageDexDeposit::PageDexDeposit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexDeposit) {
    ui->setupUi(this);

    ui->statusLabel->setVisible(false);

    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();

    iconLabelQRectBack = ui->iconLabel->geometry();
    depositFundsForLabelQRectBack = ui->depositFundsForLabel->geometry();
    depositFundsForLabelQFontBack = ui->depositFundsForLabel->font();
    tickerLabelQRectBack = ui->tickerLabel->geometry();
    tickerLabelQFontBack = ui->tickerLabel->font();
    providerLabelQRectBack = ui->providerLabel->geometry();
    providerLabelQFontBack = ui->providerLabel->font();
    contractLabelQRectBack = ui->contractLabel->geometry();
    contractLabelQFontBack = ui->contractLabel->font();
    contractAddressPushButtonQRectBack = ui->contractAddressPushButton->geometry();
    contractAddressPushButtonQFontBack = ui->contractAddressPushButton->font();
    depositAddressLabelQRectBack = ui->depositAddressLabel->geometry();
    depositAddressLabelQFontBack = ui->depositAddressLabel->font();
    depositAddressPushButtonQRectBack = ui->depositAddressPushButton->geometry();
    depositAddressPushButtonQFontBack = ui->depositAddressPushButton->font();
    minimalDepositLabelQRectBack = ui->minimalDepositLabel->geometry();
    minimalDepositLabelQFontBack = ui->minimalDepositLabel->font();
    depositFeeLabelQRectBack = ui->depositFeeLabel->geometry();
    depositFeeLabelQFontBack = ui->depositFeeLabel->font();
    confirmationLabelQRectBack = ui->confirmationLabel->geometry();
    confirmationLabelQFontBack = ui->confirmationLabel->font();

    statusLabelQRectBack = ui->statusLabel->geometry();
    statusLabelQFontBack = ui->statusLabel->font();

    setStyle();
    setScale();

    fadeTimer.setInterval(20);
    fadeTimer.stop();
    connect(&fadeTimer, &QTimer::timeout, this, &PageDexDeposit::on_copiedFadeTimer);
}

PageDexDeposit::~PageDexDeposit() {
    delete ui;
}

void PageDexDeposit::open(void *arg1) {
    entry_t entry = *(entry_t *)arg1;
    ui->tickerLabel->setText(entry.ticker);
    ui->providerLabel->setText(QString::asprintf("%s: %s",
                                                 Tr("Provider").toUtf8().data(),
                                                 entry.provider.toUtf8().data()
                                                 ));
    ui->contractAddressPushButton->setText(entry.contract);
    ui->depositAddressPushButton->setText(entry.depositAddress);
    ui->minimalDepositLabel->setText(QString::asprintf("%s: %s %s",
                                                       Tr("Minimal deposit").toUtf8().data(),
                                                       Global::Util::normaliseNumber(entry.minimalDeposit).toUtf8().data(),
                                                       entry.ticker.toUtf8().data()
                                                       ));
    ui->depositFeeLabel->setText(QString::asprintf("%s: %s %s",
                                                   Tr("Deposit Fee").toUtf8().data(),
                                                   Global::Util::normaliseNumber(entry.depositFee).toUtf8().data(),
                                                   entry.ticker.toUtf8().data()
                                                   ));
    ui->confirmationLabel->setText(QString::asprintf("%s: %s",
                                                     Tr("Confirmation").toUtf8().data(),
                                                     entry.confirmations.toUtf8().data()
                                                     ));
}

void PageDexDeposit::close() {

}

void PageDexDeposit::setScale() {
    setGeometry(Global::Layout::getLayoutGeometryScaled());
    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->iconLabel->setGeometry(Global::Layout::scaleRect(iconLabelQRectBack));
    ui->depositFundsForLabel->setGeometry(Global::Layout::scaleRect(depositFundsForLabelQRectBack));
    ui->depositFundsForLabel->setFont(Global::Layout::scaleFontOffset(depositFundsForLabelQFontBack));
    ui->tickerLabel->setGeometry(Global::Layout::scaleRect(tickerLabelQRectBack));
    ui->tickerLabel->setFont(Global::Layout::scaleFontOffset(tickerLabelQFontBack));
    ui->providerLabel->setGeometry(Global::Layout::scaleRect(providerLabelQRectBack));
    ui->providerLabel->setFont(Global::Layout::scaleFontOffset(providerLabelQFontBack));
    ui->contractLabel->setGeometry(Global::Layout::scaleRect(contractLabelQRectBack));
    ui->contractLabel->setFont(Global::Layout::scaleFontOffset(contractLabelQFontBack));
    ui->contractAddressPushButton->setGeometry(Global::Layout::scaleRect(contractAddressPushButtonQRectBack));
    ui->contractAddressPushButton->setFont(Global::Layout::scaleFontOffset(contractAddressPushButtonQFontBack));
    ui->depositAddressLabel->setGeometry(Global::Layout::scaleRect(depositAddressLabelQRectBack));
    ui->depositAddressLabel->setFont(Global::Layout::scaleFontOffset(depositAddressLabelQFontBack));
    ui->depositAddressPushButton->setGeometry(Global::Layout::scaleRect(depositAddressPushButtonQRectBack));
    ui->depositAddressPushButton->setFont(Global::Layout::scaleFontOffset(depositAddressPushButtonQFontBack));
    ui->minimalDepositLabel->setGeometry(Global::Layout::scaleRect(minimalDepositLabelQRectBack));
    ui->minimalDepositLabel->setFont(Global::Layout::scaleFontOffset(minimalDepositLabelQFontBack));
    ui->depositFeeLabel->setGeometry(Global::Layout::scaleRect(depositFeeLabelQRectBack));
    ui->depositFeeLabel->setFont(Global::Layout::scaleFontOffset(depositFeeLabelQFontBack));
    ui->confirmationLabel->setGeometry(Global::Layout::scaleRect(confirmationLabelQRectBack));
    ui->confirmationLabel->setFont(Global::Layout::scaleFontOffset(confirmationLabelQFontBack));

    ui->statusLabel->setGeometry(Global::Layout::scaleRect(statusLabelQRectBack));
    ui->statusLabel->setFont(Global::Layout::scaleFontOffset(statusLabelQFontBack));
}

void PageDexDeposit::setStyle() {

}

void PageDexDeposit::loop() {

}

void PageDexDeposit::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::DEX);
}

void PageDexDeposit::on_copiedFadeTimer() {
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
}

void PageDexDeposit::on_contractAddressPushButton_clicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->contractAddressPushButton->text());
    ui->statusLabel->setText(Tr("Cpoied to clipboard"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

void PageDexDeposit::on_depositAddressPushButton_clicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->depositAddressPushButton->text());
    ui->statusLabel->setText(Tr("Cpoied to clipboard"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

