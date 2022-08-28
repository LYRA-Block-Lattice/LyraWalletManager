#include "pageimportwallet.h"
#include "ui_pageimportwallet.h"

#include <QMessageBox>

#include "style.h"
#include "global.h"

PageImportWallet::PageImportWallet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageImportWallet) {
    ui->setupUi(this);
    ui->ImportAccountFrame->setStyleSheet("background-color: #FCD535;");

    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerLabelQRectBack = ui->headerLabel->geometry();
    headerLabelQFontBack = ui->headerLabel->font();
    walletNameLineEditQRectBack = ui->walletNameLineEdit->geometry();
    walletNameLineEditQFontBack = ui->walletNameLineEdit->font();
    importWalletPushButtonQRectBack = ui->importWalletPushButton->geometry();
    importWalletPushButtonQFontBack = ui->importWalletPushButton->font();

    setScale();
    setStyle();
    ui->ImportAccountFrame->repaint();
}

PageImportWallet::~PageImportWallet() {
    delete ui;
}

void PageImportWallet::open() {
    ui->walletNameLineEdit->clear();
}

void PageImportWallet::setScale() {
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    QRect rect = Global::Layout::getLayoutGeometryScaled();
    rect.setY(rect.y() + Global::Layout::getHeaderHeight());
    ui->ImportAccountFrame->setGeometry(rect);

    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerLabel->setGeometry(Global::Layout::scaleRect(headerLabelQRectBack));
    ui->headerLabel->setFont(Global::Layout::scaleFont(headerLabelQFontBack));
    ui->walletNameLineEdit->setGeometry(Global::Layout::scaleRect(walletNameLineEditQRectBack));
    ui->walletNameLineEdit->setFont(Global::Layout::scaleFont(walletNameLineEditQFontBack));
    ui->importWalletPushButton->setGeometry(Global::Layout::scaleRect(importWalletPushButtonQRectBack));
    ui->importWalletPushButton->setFont(Global::Layout::scaleFont(importWalletPushButtonQFontBack));
}

void PageImportWallet::setStyle() {
    Style::setButtonManagerPageStyle(ui->backPushButton);
    Style::setLabelStyle(ui->headerLabel);
    Style::setLineEditManagerPageStyle(ui->walletNameLineEdit);
    Style::setButtonManagerPageStyle(ui->importWalletPushButton);
}

void PageImportWallet::setVisibility(bool v) {
    this->setVisible(v);
}

bool PageImportWallet::getVisibility() {
    return isVisible();
}

void PageImportWallet::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::OPEN_WALLET);
}


void PageImportWallet::on_importWalletPushButton_clicked() {
    if(StorageInternal::walletImport(ui->walletNameLineEdit->text()))
        Global::Page::goManagerPage(Global::Page::OPEN_WALLET);
}


void PageImportWallet::on_walletNameLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->walletNameLineEdit->style()->polish(ui->walletNameLineEdit);
}

