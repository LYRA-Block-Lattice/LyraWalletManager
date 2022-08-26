#include "pagedexwithdraw.h"
#include "ui_pagedexwithdraw.h"

#include <QGraphicsOpacityEffect>
#include <QClipboard>

#include "global.h"
#include "color.h"
#include "style.h"

#include "external/externaladdressvalidator.h"
#include "translate/translate.h"

PageDexWithdraw::PageDexWithdraw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexWithdraw) {
    ui->setupUi(this);

    ui->statusLabel->setVisible(false);

    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();

    iconLabelQRectBack = ui->iconLabel->geometry();
    withdrawFundsForLabelQRectBack = ui->withdrawFundsForLabel->geometry();
    withdrawFundsForLabelQFontBack = ui->withdrawFundsForLabel->font();
    tickerLabelQRectBack = ui->tickerLabel->geometry();
    tickerLabelQFontBack = ui->tickerLabel->font();
    providerLabelQRectBack = ui->providerLabel->geometry();
    providerLabelQFontBack = ui->providerLabel->font();
    contractLabelQRectBack = ui->contractLabel->geometry();
    contractLabelQFontBack = ui->contractLabel->font();
    contractAddressPushButtonQRectBack = ui->contractAddressPushButton->geometry();
    contractAddressPushButtonQFontBack = ui->contractAddressPushButton->font();
    withdrawFeeLabelQRectBack = ui->withdrawFeeLabel->geometry();
    withdrawFeeLabelQFontBack = ui->withdrawFeeLabel->font();
    addressToLabelQRectBack = ui->addressToLabel->geometry();
    addressToLabelQFontBack = ui->addressToLabel->font();
    recipientAddressLineEditQRectBack = ui->recipientAddressLineEdit->geometry();
    recipientAddressLineEditQFontBack = ui->recipientAddressLineEdit->font();
    qrPushButtonQRectBack = ui->qrPushButton->geometry();
    qrPushButtonQSizeBack = ui->qrPushButton->iconSize();
    amountLineEditQRectBack = ui->amountLineEdit->geometry();
    amountLineEditQFontBack = ui->amountLineEdit->font();
    maximumPushButtonQRectBack = ui->maximumPushButton->geometry();
    maximumPushButtonQSizeBack = ui->maximumPushButton->iconSize();
    withdrawPushButtonQRectBack = ui->withdrawPushButton->geometry();
    withdrawPushButtonQFontBack = ui->withdrawPushButton->font();

    statusLabelQRectBack = ui->statusLabel->geometry();
    statusLabelQFontBack = ui->statusLabel->font();

    setStyle();
    setScale();

    fadeTimer.setInterval(20);
    fadeTimer.stop();
    connect(&fadeTimer, &QTimer::timeout, this, &PageDexWithdraw::on_copiedFadeTimer);
}

PageDexWithdraw::~PageDexWithdraw() {
    delete ui;
}

void PageDexWithdraw::open(void *arg1) {
    Entry = *(entry_t *)arg1;
    ui->tickerLabel->setText(Entry.ticker);
    ui->providerLabel->setText(QString::asprintf("%s: %s",
                                                 Tr("Provider").toUtf8().data(),
                                                 Entry.provider.toUtf8().data()
                                                 ));
    ui->contractAddressPushButton->setText(Entry.contract);
    ui->withdrawFeeLabel->setText(QString::asprintf("%s: %s %s",
                                                   Tr("Withdraw Fee").toUtf8().data(),
                                                   Global::Util::normaliseNumber(Entry.withdrawFee).toUtf8().data(),
                                                   Entry.ticker.toUtf8().data()
                                                   ));
    ui->amountLineEdit->setPlaceholderText(QString::asprintf("%s: %s %s",
                                                             Tr("Available").toUtf8().data(),
                                                             Global::Util::normaliseNumber(Entry.amountInAccount).toUtf8().data(),
                                                             Entry.ticker.toUtf8().data()
                                                             ));
}

void PageDexWithdraw::close() {

}

void PageDexWithdraw::setScale() {
    setGeometry(Global::Layout::getLayoutGeometryScaled());
    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->iconLabel->setGeometry(Global::Layout::scaleRect(iconLabelQRectBack));
    ui->withdrawFundsForLabel->setGeometry(Global::Layout::scaleRect(withdrawFundsForLabelQRectBack ));
    ui->withdrawFundsForLabel->setFont(Global::Layout::scaleFontOffset(withdrawFundsForLabelQFontBack));
    ui->tickerLabel->setGeometry(Global::Layout::scaleRect(tickerLabelQRectBack));
    ui->tickerLabel->setFont(Global::Layout::scaleFontOffset(tickerLabelQFontBack));
    ui->providerLabel->setGeometry(Global::Layout::scaleRect(providerLabelQRectBack));
    ui->providerLabel->setFont(Global::Layout::scaleFontOffset(providerLabelQFontBack));
    ui->contractLabel->setGeometry(Global::Layout::scaleRect(contractLabelQRectBack));
    ui->contractLabel->setFont(Global::Layout::scaleFontOffset(contractLabelQFontBack));
    ui->contractAddressPushButton->setGeometry(Global::Layout::scaleRect(contractAddressPushButtonQRectBack));
    ui->contractAddressPushButton->setFont(Global::Layout::scaleFontOffset(contractAddressPushButtonQFontBack));
    ui->withdrawFeeLabel->setGeometry(Global::Layout::scaleRect(withdrawFeeLabelQRectBack));
    ui->withdrawFeeLabel->setFont(Global::Layout::scaleFontOffset(withdrawFeeLabelQFontBack));
    ui->addressToLabel->setGeometry(Global::Layout::scaleRect(addressToLabelQRectBack));
    ui->addressToLabel->setFont(Global::Layout::scaleFontOffset(addressToLabelQFontBack));
    ui->recipientAddressLineEdit->setGeometry(Global::Layout::scaleRect(recipientAddressLineEditQRectBack));
    ui->recipientAddressLineEdit->setFont(Global::Layout::scaleFontOffset(recipientAddressLineEditQFontBack));
    ui->qrPushButton->setGeometry(Global::Layout::scaleRect(qrPushButtonQRectBack));
    ui->qrPushButton->setIconSize(Global::Layout::scaleSize(qrPushButtonQSizeBack));
    ui->amountLineEdit->setGeometry(Global::Layout::scaleRect(amountLineEditQRectBack));
    ui->amountLineEdit->setFont(Global::Layout::scaleFontOffset(amountLineEditQFontBack));
    ui->maximumPushButton->setGeometry(Global::Layout::scaleRect(maximumPushButtonQRectBack));
    ui->maximumPushButton->setIconSize(Global::Layout::scaleSize(maximumPushButtonQSizeBack));
    ui->withdrawPushButton->setGeometry(Global::Layout::scaleRect(withdrawPushButtonQRectBack));
    ui->withdrawPushButton->setFont(Global::Layout::scaleFontOffset(withdrawPushButtonQFontBack));

    ui->statusLabel->setGeometry(Global::Layout::scaleRect(statusLabelQRectBack));
    ui->statusLabel->setFont(Global::Layout::scaleFontOffset(statusLabelQFontBack));
}

void PageDexWithdraw::setStyle() {

}

void PageDexWithdraw::loop() {

}


bool PageDexWithdraw::checkCameraAvailability() {
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void PageDexWithdraw::on_recipientAddressLineEdit_textChanged(const QString &arg1) {
    ui->recipientAddressLineEdit->style()->polish(ui->recipientAddressLineEdit);
    if((!Entry.ticker.compare("TRX") || !Entry.provider.compare("TRC-20")) && ExternalAddressValidator::tron(arg1))
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit);
    else if ((!Entry.ticker.compare("ETH") || !Entry.provider.compare("ERC-20")) && ExternalAddressValidator::ethereum(arg1)) {
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit);
    } else
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit, "red");
}

void PageDexWithdraw::on_qrPushButton_clicked() {
    if(checkCameraAvailability()) {
        camera = new Camera(this);
        connect(camera, &Camera::qrRead, this, [=](QString qr) {
            ui->recipientAddressLineEdit->setText(qr);
            ui->qrPushButton->setEnabled(true);
        });
        connect(camera, &Camera::windowClosed, this, [=]{
            ui->qrPushButton->setEnabled(true);
        });
        camera->show();
        ui->qrPushButton->setEnabled(false);
    } else {
        QMessageBox::critical(this, "ERROR", Tr("No camera device detected"));
    }
}

void PageDexWithdraw::on_amountLineEdit_textChanged(const QString &arg1) {
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

void PageDexWithdraw::on_maximumPushButton_clicked() {
    if(Entry.amountInAccount > Entry.withdrawFee)
        ui->amountLineEdit->setText(Global::Util::normaliseNumber(Entry.amountInAccount - Entry.withdrawFee));
    else {
        ui->amountLineEdit->setText("");
        ui->statusLabel->setText(Tr("Insuficient funds"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
    }
}

void PageDexWithdraw::on_withdrawPushButton_clicked() {

}


void PageDexWithdraw::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::DEX);
}

void PageDexWithdraw::on_copiedFadeTimer() {
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

void PageDexWithdraw::on_contractAddressPushButton_clicked() {
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->contractAddressPushButton->text());
    ui->statusLabel->setText(Tr("Cpoied to clipboard"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

