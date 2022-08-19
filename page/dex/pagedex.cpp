#include "pagedex.h"
#include "ui_pagedex.h"

#include <QScroller>

#include "global.h"
#include "color.h"
#include "style.h"

#include "page/account/pageaccount.h"

PageDex::PageDex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDex) {
    ui->setupUi(this);
    Parent = parent;

    // Backup items geometry, size and font.
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();


    page = new PageDexListMain(this);
    ui->scrollArea->setWidget(page);
    page->append("LYR", 0.1, 0.2);
    page->append("TRX", 0.3, 0.4);
    page->repaint();
    connect(page, &PageDexListMain::setupPushButton, this, [=](int index) {
        qDebug() << "Setup" << index;
    });
    connect(page, &PageDexListMain::spotToDexPushButton, this, [=](int index) {
        qDebug() << "Sppt to DEX" << index;
    });
    connect(page, &PageDexListMain::dexToSpotPushButton, this, [=](int index) {
        qDebug() << "DEX to Spot" << index;
    });
    connect(page, &PageDexListMain::depositPushButton, this, [=](int index) {
        qDebug() << "Deposit" << index;
    });
    connect(page, &PageDexListMain::withdrawPushButton, this, [=](int index) {
        qDebug() << "Withdraw" << index;
    });

    setScale();
    setStyle();

    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);
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
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->scrollArea->setGeometry(QRect(0, Global::Layout::scaleValueX(56), Parent->geometry().width(), Parent->geometry().height() - Global::Layout::scaleValueX(56) - Global::Layout::scaleValueX(MENU_BAR_HEIGHT + 10)));

    page->setScale();
}

void PageDex::setStyle() {
    page->setStyle();
}

void PageDex::loop() {
    if(AccountListChangedCount != Global::Account::getAccountListChangedCount()) {
        AccountListChangedCount = Global::Account::getAccountListChangedCount();
    }
}


