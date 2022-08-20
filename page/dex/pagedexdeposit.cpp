#include "pagedexdeposit.h"
#include "ui_pagedexdeposit.h"

#include "global.h"
#include "color.h"
#include "style.h"

PageDexDeposit::PageDexDeposit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexDeposit) {
    ui->setupUi(this);
}

PageDexDeposit::~PageDexDeposit() {
    delete ui;
}
