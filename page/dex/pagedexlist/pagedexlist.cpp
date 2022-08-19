#include "pagedexlist.h"
#include "ui_pagedexlist.h"

#include <QPixmap>

#include "global.h"
#include "style.h"

PageDexList::PageDexList(entry_t entry,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexList) {
    ui->setupUi(this);
    Parent = parent;
    Index = entry.Index;
    setTicker(entry.Ticker);
    setSpotAmount(entry.SpotAmount);
    setDexAmount(entry.DexAmount);

    thisQRectBack = this->geometry();
    groupBoxQRectBack = ui->groupBox->geometry();

    tickerIconLabelQRectBack = ui->tickerIconLabel->geometry();
    //tickerIconLabelQSizeBack = ui->tickerIconLabel->size();

    tickerTextLabelQRectBack = ui->tickerTextLabel->geometry();
    tickerTextLabelQFontBack = ui->tickerTextLabel->font();
    labelQRectBack = ui->label->geometry();
    labelQFontBack = ui->label->font();
    spotLabelQRectBack = ui->spotLabel->geometry();
    spotLabelQFontBack = ui->spotLabel->font();
    dexLabelQRectBack = ui->dexLabel->geometry();
    dexLabelQFontBack = ui->dexLabel->font();
    spotAmountLabelQRectBack = ui->spotAmountLabel->geometry();
    spotAmountLabelQFontBack = ui->spotAmountLabel->font();
    dexAmountLabelQRectBack = ui->dexAmountLabel->geometry();
    dexAmountLabelQFontBack = ui->dexAmountLabel->font();

    setupPushButtonQRectBack = ui->setupPushButton->geometry();
    setupPushButtonQSizeBack = ui->setupPushButton->iconSize();
    spotToDexPushButtonQRectBack = ui->spotToDexPushButton->geometry();
    spotToDexPushButtonQSizeBack = ui->spotToDexPushButton->iconSize();
    dexToSpotPushButtonQRectBack = ui->dexToSpotPushButton->geometry();
    dexToSpotPushButtonQSizeBack = ui->dexToSpotPushButton->iconSize();

    depositPushButtonQRectBack = ui->depositPushButton->geometry();
    depositPushButtonQFontBack = ui->depositPushButton->font();
    withdrawPushButtonQRectBack = ui->withdrawPushButton->geometry();
    withdrawPushButtonQFontBack = ui->withdrawPushButton->font();

    setStyle();
    setScale();
}

PageDexList::~PageDexList() {
    delete ui;
}

void PageDexList::setScale() {
    setGeometry(Global::Layout::scaleRect(thisQRectBack));
    ui->groupBox->setGeometry(Global::Layout::scaleRect(groupBoxQRectBack));

    ui->tickerIconLabel->setGeometry(Global::Layout::scaleRect(tickerIconLabelQRectBack));
    //ui->tickerIconLabel->setSize(tickerIconLabelQSizeBack);

    ui->tickerTextLabel->setGeometry(Global::Layout::scaleRect(tickerTextLabelQRectBack));
    ui->tickerTextLabel->setFont(Global::Layout::scaleFontOffset(tickerTextLabelQFontBack));
    ui->label->setGeometry(Global::Layout::scaleRect(labelQRectBack));
    ui->label->setFont(Global::Layout::scaleFontOffset(labelQFontBack));
    ui->spotLabel->setGeometry(Global::Layout::scaleRect(spotLabelQRectBack));
    ui->spotLabel->setFont(Global::Layout::scaleFontOffset(spotLabelQFontBack));
    ui->dexLabel->setGeometry(Global::Layout::scaleRect(dexLabelQRectBack));
    ui->dexLabel->setFont(Global::Layout::scaleFontOffset(dexLabelQFontBack));
    ui->spotAmountLabel->setGeometry(Global::Layout::scaleRect(spotAmountLabelQRectBack));
    ui->spotAmountLabel->setFont(Global::Layout::scaleFontOffset(spotAmountLabelQFontBack));
    ui->dexAmountLabel->setGeometry(Global::Layout::scaleRect(dexAmountLabelQRectBack));
    ui->dexAmountLabel->setFont(Global::Layout::scaleFontOffset(dexAmountLabelQFontBack));

    ui->setupPushButton->setGeometry(Global::Layout::scaleRect(setupPushButtonQRectBack));
    ui->setupPushButton->setIconSize(Global::Layout::scaleSize(setupPushButtonQSizeBack));
    ui->spotToDexPushButton->setGeometry(Global::Layout::scaleRect(spotToDexPushButtonQRectBack));
    ui->spotToDexPushButton->setIconSize(Global::Layout::scaleSize(spotToDexPushButtonQSizeBack));
    ui->dexToSpotPushButton->setGeometry(Global::Layout::scaleRect(dexToSpotPushButtonQRectBack));
    ui->dexToSpotPushButton->setIconSize(Global::Layout::scaleSize(dexToSpotPushButtonQSizeBack));

    ui->depositPushButton->setGeometry(Global::Layout::scaleRect(depositPushButtonQRectBack));
    ui->depositPushButton->setFont(Global::Layout::scaleFontOffset(depositPushButtonQFontBack));
    ui->withdrawPushButton->setGeometry(Global::Layout::scaleRect(withdrawPushButtonQRectBack));
    ui->withdrawPushButton->setFont(Global::Layout::scaleFontOffset(withdrawPushButtonQFontBack));
}

void PageDexList::setStyle() {
    Style::setButtonDefaultStyle(ui->depositPushButton);
    Style::setButtonDefaultStyle(ui->withdrawPushButton);
}

void PageDexList::setTicker(QString ticker) {
    Ticker = ticker;
    ui->tickerIconLabel->setPixmap(QPixmap(Global::TickerIcon::get(ticker)));
    ui->tickerTextLabel->setText(Global::Util::tickerToTokenName(ticker));
}

void PageDexList::setSpotAmount(double amount) {
    SpotAmount = amount;
    ui->spotAmountLabel->setText(QString::asprintf("%s %s", Global::Util::normaliseNumber(amount).toUtf8().data(), Ticker.toUtf8().data()));
}

void PageDexList::setDexAmount(double amount) {
    DexAmount = amount;
    ui->dexAmountLabel->setText(QString::asprintf("%s %s", Global::Util::normaliseNumber(amount).toUtf8().data(), Ticker.toUtf8().data()));
}

void PageDexList::on_setupPushButton_clicked() {
    emit setupPushButton(Index);
}

void PageDexList::on_spotToDexPushButton_clicked() {
    emit spotToDexPushButton(Index);
}

void PageDexList::on_dexToSpotPushButton_clicked() {
    emit dexToSpotPushButton(Index);
}

void PageDexList::on_depositPushButton_clicked() {
    emit depositPushButton(Index);
}

void PageDexList::on_withdrawPushButton_clicked() {
    emit withdrawPushButton(Index);
}

