#include "pagestaking.h"
#include "ui_pagestaking.h"

#include "global.h"
#include "color.h"
#include "style.h"

PageStaking::PageStaking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageStaking) {
    ui->setupUi(this);


    // Backup items geometry, size and font.
    headerFrameQRectBack = ui->headerFrame->geometry();

    setScale();
    setStyle();
}

PageStaking::~PageStaking() {
    delete ui;
}

void PageStaking::open() {

}

void PageStaking::close() {

}

void PageStaking::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
}

void PageStaking::setStyle() {

}

void PageStaking::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
    }
}
