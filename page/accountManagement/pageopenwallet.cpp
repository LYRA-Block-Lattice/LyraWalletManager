#include "pageopenwallet.h"
#include "ui_pageopenwallet.h"

#include "style.h"
#include "global.h"
#include "storage/storagesettings.h"

PageOpenWallet::PageOpenWallet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageOpenWallet) {
    ui->setupUi(this);
    ui->OpenWalletFrame->setStyleSheet("background-color: #FCD535;");

    // Backup items geometry, size and font.
    //backPushButtonQRectBack = ui->backPushButton->geometry();
    //backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerLabelQRectBack = ui->headerLabel->geometry();
    headerLabelQFontBack = ui->headerLabel->font();
    walletNameLineEditQRectBack = ui->walletNameLineEdit->geometry();
    walletNameLineEditQFontBack = ui->walletNameLineEdit->font();
    walletPasswordLineEditQRectBack = ui->walletPasswordLineEdit->geometry();
    walletPasswordLineEditQFontBack = ui->walletPasswordLineEdit->font();
    showHidePushButtonQRectBack = ui->showHidePushButton->geometry();
    showHidePushButtonQSizeBack = ui->showHidePushButton->iconSize();
    openWalletPushButtonQRectBack = ui->openWalletPushButton->geometry();
    openWalletPushButtonQFontBack = ui->openWalletPushButton->font();
    newWalletPushButtonQRectBack = ui->newWalletPushButton->geometry();
    newWalletPushButtonQFontBack = ui->newWalletPushButton->font();
    importWalletPushButtonQRectBack = ui->importWalletPushButton->geometry();
    importWalletPushButtonQFontBack = ui->importWalletPushButton->font();

    connect(ui->walletPasswordLineEdit, &QLineEdit::returnPressed, this, [this] {
        on_openWalletPushButton_clicked();
    });


    setScale();
    setStyle();
    retranslateUi();
    ui->OpenWalletFrame->repaint();
}

PageOpenWallet::~PageOpenWallet() {
    delete ui;
}

void PageOpenWallet::open() {
    ui->walletNameLineEdit->clear();
    ui->walletPasswordLineEdit->clear();
}

void PageOpenWallet::setScale() {
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    QRect rect = Global::Layout::getLayoutGeometryScaled();
    rect.setY(rect.y() + Global::Layout::getHeaderHeight());
    ui->OpenWalletFrame->setGeometry(rect);

    //ui->backPushButton->setGeometry(Global::scaleRect(backPushButtonQRectBack));
    //ui->backPushButton->setIconSize(Global::scaleSize(backPushButtonQSizeBack));
    ui->headerLabel->setGeometry(Global::Layout::scaleRect(headerLabelQRectBack));
    ui->headerLabel->setFont(Global::Layout::scaleFont(headerLabelQFontBack));
    ui->walletNameLineEdit->setGeometry(Global::Layout::scaleRect(walletNameLineEditQRectBack));
    ui->walletNameLineEdit->setFont(Global::Layout::scaleFont(walletNameLineEditQFontBack));
    ui->walletPasswordLineEdit->setGeometry(Global::Layout::scaleRect(walletPasswordLineEditQRectBack));
    ui->walletPasswordLineEdit->setFont(Global::Layout::scaleFont(walletPasswordLineEditQFontBack));
    ui->showHidePushButton->setGeometry(Global::Layout::scaleRect(showHidePushButtonQRectBack));
    ui->showHidePushButton->setIconSize(Global::Layout::scaleSize(showHidePushButtonQSizeBack));
    ui->openWalletPushButton->setGeometry(Global::Layout::scaleRect(openWalletPushButtonQRectBack));
    ui->openWalletPushButton->setFont(Global::Layout::scaleFont(openWalletPushButtonQFontBack));
    ui->newWalletPushButton->setGeometry(Global::Layout::scaleRect(newWalletPushButtonQRectBack));
    ui->newWalletPushButton->setFont(Global::Layout::scaleFont(newWalletPushButtonQFontBack));
    ui->importWalletPushButton->setGeometry(Global::Layout::scaleRect(importWalletPushButtonQRectBack));
    ui->importWalletPushButton->setFont(Global::Layout::scaleFont(importWalletPushButtonQFontBack));
}

void PageOpenWallet::setStyle() {
    //Style::setButtonStyle(ui->backPushButton);
    /*Style::setLabelStyle(ui->headerLabel);
    Style::setLineEditlStyle(ui->walletNameLineEdit);
    Style::setLineEditlStyle(ui->walletPasswordLineEdit);
    Style::setButtonStyle(ui->showHidePushButton);
    Style::setButtonStyle(ui->openWalletPushButton);
    Style::setButtonStyle(ui->newWalletPushButton);
    Style::setButtonStyle(ui->importWalletPushButton);*/
}

void PageOpenWallet::setVisibility(bool v) {
    ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
    ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    this->setVisible(v);
}

bool PageOpenWallet::getVisibility() {
    return isVisible();
}

void PageOpenWallet::retranslateUi() {
    //Style::setButtonStyle(ui->backPushButton);
    ui->walletNameLineEdit->setPlaceholderText(tr("Type wallet name"));
    ui->walletPasswordLineEdit->setPlaceholderText(tr("Type wallet password"));
    ui->openWalletPushButton->setText(tr("OPEN WALLET"));
    ui->newWalletPushButton->setText(tr("NEW WALLET"));
    ui->importWalletPushButton->setText(tr("IMPORT WALLET"));
}

void PageOpenWallet::on_showHidePushButton_clicked() {
    if (ui->walletPasswordLineEdit->echoMode() == QLineEdit::Password) {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordShown.png"));
    } else {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    }
}


void PageOpenWallet::on_openWalletPushButton_clicked(){
    Global::Wallet::Name::set(ui->walletNameLineEdit->text());
    StorageCommon::storageError_e err = StorageInternal::walletRead(ui->walletPasswordLineEdit->text());
    if(Global::Error::show(this, err))
        return;
    if(Global::Account::getAccountList().count() == 0)
        Global::Page::goManagerPage(Global::Page::NEW_ACCOUNT);
    else
        Global::Page::goManagerPage(Global::Page::ACCOUNT);
    StorageSettings::createDefault();
    Global::Network::setNetwork(StorageSettings::get("network", "TESTNET"));
    Global::Account::setSelectedAccount(StorageSettings::get("lastAccount"));
}

void PageOpenWallet::on_newWalletPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::NEW_WALLET);
}

void PageOpenWallet::on_importWalletPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::IMPORT_WALLET);
}


void PageOpenWallet::on_walletNameLineEdit_textChanged(const QString &arg1) {
    ui->walletNameLineEdit->style()->polish(ui->walletNameLineEdit);
    if(Global::Check::checkWalletFileNameValid(arg1))
        Style::setLineEditlWhiteStyle(ui->walletNameLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->walletNameLineEdit, "red");
}

void PageOpenWallet::on_walletPasswordLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->walletPasswordLineEdit->style()->polish(ui->walletPasswordLineEdit);
    if(Global::Check::password(ui->walletPasswordLineEdit->text()))
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit, "red");
}


