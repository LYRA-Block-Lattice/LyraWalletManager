#include "pageswap.h"
#include "ui_pageswap.h"

#include "global.h"
#include "color.h"
#include "style.h"

PageSwap::PageSwap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSwap) {
    ui->setupUi(this);


    // Backup items geometry, size and font.
    headerFrameQRectBack = ui->headerFrame->geometry();

    setScale();
    setStyle();
}

PageSwap::~PageSwap() {
    delete ui;
}

void PageSwap::open() {

}

void PageSwap::close() {

}

void PageSwap::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
}

void PageSwap::setStyle() {

}

void PageSwap::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
    }
}
