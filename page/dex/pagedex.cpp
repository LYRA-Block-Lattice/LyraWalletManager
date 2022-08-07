#include "pagedex.h"
#include "ui_pagedex.h"

#include "global.h"
#include "color.h"
#include "style.h"

PageDex::PageDex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDex) {
    ui->setupUi(this);

    // Backup items geometry, size and font.
    headerFrameQRectBack = ui->headerFrame->geometry();

    setScale();
    setStyle();
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
}

void PageDex::setStyle() {

}

void PageDex::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
    }
}


