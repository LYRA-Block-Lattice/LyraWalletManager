#include "pageswap.h"
#include "ui_pageswap.h"

#include "global.h"
#include "color.h"
#include "style.h"

#include "wallet/history.h"

PageSwap::PageSwap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSwap) {
    ui->setupUi(this);

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

    amountSentLineEditQRectBack = ui->amountSentLineEdit->geometry();
    amountSentLineEditQFontBack = ui->amountSentLineEdit->font();
    tokenSentComboBoxQRectBack = ui->tokenSentComboBox->geometry();
    tokenSentComboBoxQFontBack = ui->tokenSentComboBox->font();
    tokenSentComboBoxQSizeBack = ui->tokenSentComboBox->iconSize();
    availableSentLabelQRectBack = ui->availableSentLabel->geometry();
    availableSentLabelQFontBack = ui->availableSentLabel->font();

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



    setScale();
    setStyle();

    ui->tokenSentComboBox->setView(new QListView);
    ui->tokenReceiveComboBox->setView(new QListView);
}

PageSwap::~PageSwap() {
    delete ui;
}

void PageSwap::open() {
    sendSelectedTicker = "";
    receiveSelectedTicker = "";
    populateSendTickers("LYR");
    ui->removeSharePushButton->setVisible(false);
    on_swapSelectPushButton_clicked();
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

    //ui->receiveGridWidget->setGeometry(Global::Layout::scaleRect(ui->receiveGridWidget->geometry()));
    //ui->sendGridWidget->setGeometry(Global::Layout::scaleRect(ui->sendGridWidget->geometry()));
    /*ui->receiveGridFrame->setGeometry(0, 0,
                                       Global::Layout::scaleRect(ui->receiveGridFrame->geometry()).width(),
                                       Global::Layout::scaleRect(ui->receiveGridFrame->geometry()).height());*/
    /*ui->sendGridFrame->setGeometry(0, 0,
                                    Global::Layout::scaleRect(ui->sendGridFrame->geometry()).width(),
                                    Global::Layout::scaleRect(ui->sendGridFrame->geometry()).height());*/

    ui->amountSentLineEdit->setGeometry(Global::Layout::scaleRect(amountSentLineEditQRectBack));
    ui->amountSentLineEdit->setFont(Global::Layout::scaleFontOffset(amountSentLineEditQFontBack));
    ui->tokenSentComboBox->setGeometry(Global::Layout::scaleRect(tokenSentComboBoxQRectBack));
    ui->tokenSentComboBox->setFont(Global::Layout::scaleFontOffset(tokenSentComboBoxQFontBack));
    ui->tokenSentComboBox->setIconSize(Global::Layout::scaleSize(tokenSentComboBoxQSizeBack));
    ui->availableSentLabel->setGeometry(Global::Layout::scaleRect(availableSentLabelQRectBack));
    ui->availableSentLabel->setFont(Global::Layout::scaleFontOffset(availableSentLabelQFontBack));

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


}

void PageSwap::setStyle() {

}

void PageSwap::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
    }
}

void PageSwap::populateSendTickers(QString txt) {
    populatingTickers = true;
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        QList<RpcClass::History::entry_t> tmpListEntrys = historyInst->getHistory();
        QList<QPair<QString, double>> balances = tmpListEntrys.last().Balances;
        QPair<QString, double> balance;
        if(!txt.compare("LYR")) {
            ui->tokenSentComboBox->clear();
            ui->tokenSentComboBox->addItem(QPixmap(Global::TickerIcon::get("LYR")), "LYR");
            ui->tokenReceiveComboBox->clear();
            bool first = true;
            foreach(balance, balances) {
                ui->tokenReceiveComboBox->addItem(QPixmap(Global::TickerIcon::get(balance.first)), balance.first);
                if(balance.first.compare("LYR")) {
                    if(first) {
                        first = false;
                        ui->tokenReceiveComboBox->setCurrentText(balance.first);
                        ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", "Balance", Global::Util::normaliseNumber(balance.second).toUtf8().data()));
                    }
                } else {
                    ui->availableSentLabel->setText(QString::asprintf("%s: %s", "Balance", Global::Util::normaliseNumber(balance.second).toUtf8().data()));
                }
            }
        } else {
            foreach(balance, balances) {
                if(!balance.first.compare(txt)) {
                    ui->availableSentLabel->setText(QString::asprintf("%s: %s", "Balance", Global::Util::normaliseNumber(balance.second).toUtf8().data()));
                    break;
                }
            }
        }
    }
    populatingTickers = false;
}

void PageSwap::on_swapSelectPushButton_clicked() {
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

    ui->swapPushButton->setText("START SWAP");
    ui->removeSharePushButton->setVisible(false);
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

    ui->swapPushButton->setText("ADD LIQUIDITY");
    ui->removeSharePushButton->setVisible(true);
}

void PageSwap::on_amountSentLineEdit_textChanged(const QString &arg1) {
    ui->amountSentLineEdit->style()->polish(ui->amountSentLineEdit);
    QString s = arg1;
    bool validate;
    s.remove(',').toDouble(&validate);
    if(s.length() > 0) {
        if(!validate) {
            ui->amountSentLineEdit->setText(Global::Util::normaliseNumber(s.left(s.length() - 1), false));
        } else {
            ui->amountSentLineEdit->setText(Global::Util::normaliseNumber(s.remove(','), false));
        }
    }
}

void PageSwap::on_tokenSentComboBox_currentTextChanged(const QString &arg1) {
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
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    bool first = true;
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        QList<RpcClass::History::entry_t> tmpListEntrys = historyInst->getHistory();
        QList<QPair<QString, double>> balances = tmpListEntrys.last().Balances;
        QPair<QString, double> balance;
        if(!arg1.compare("LYR")) {
            ui->tokenReceiveComboBox->clear();
            ui->tokenReceiveComboBox->addItem(QPixmap(Global::TickerIcon::get("LYR")), "LYR");
            ui->tokenSentComboBox->clear();
            foreach(balance, balances) {
                ui->tokenSentComboBox->addItem(QPixmap(Global::TickerIcon::get(balance.first)), balance.first);
                if(balance.first.compare("LYR")) {
                    if(first) {
                        first = false;
                        ui->tokenSentComboBox->setCurrentText(balance.first);
                        ui->availableSentLabel->setText(QString::asprintf("%s: %s", "Balance", Global::Util::normaliseNumber(balance.second).toUtf8().data()));
                    }
                } else {
                    ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", "Balance", Global::Util::normaliseNumber(balance.second).toUtf8().data()));
                }
            }
        } else {
            foreach(balance, balances) {
                if(!balance.first.compare(arg1)) {
                    ui->availableReceiveLabel->setText(QString::asprintf("%s: %s", "Balance", Global::Util::normaliseNumber(balance.second).toUtf8().data()));
                    break;
                }
            }
        }
    }
    populatingTickers = false;
}

void PageSwap::on_swapPushButton_clicked() {

}


void PageSwap::on_removeLiquidityPushButton_clicked() {

}

