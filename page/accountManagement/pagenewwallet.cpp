#include "pagenewwallet.h"
#include "ui_pagenewwallet.h"

#include "style.h"
#include "global.h"
#include "storage/storageinternal.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>

PageNewWallet::PageNewWallet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageNewWallet) {
    ui->setupUi(this);
    ui->NewWalletFrame->setStyleSheet("background-color: #FCD535;");

    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerLabelQRectBack = ui->headerLabel->geometry();
    headerLabelQFontBack = ui->headerLabel->font();
    walletNameLineEditQRectBack = ui->walletNameLineEdit->geometry();
    walletNameLineEditQFontBack = ui->walletNameLineEdit->font();
    walletPasswordLineEditQRectBack = ui->walletPasswordLineEdit->geometry();
    walletPasswordLineEditQFontBack = ui->walletPasswordLineEdit->font();
    walletPasswordLineEdit2QRectBack = ui->walletPasswordLineEdit_2->geometry();
    walletPasswordLineEdit2QFontBack = ui->walletPasswordLineEdit_2->font();
    showHidePushButtonQRectBack = ui->showHidePushButton->geometry();
    showHidePushButtonQSizeBack = ui->showHidePushButton->iconSize();
    createWalletPushButtonQRectBack = ui->createWalletPushButton->geometry();
    createWalletPushButtonQFontBack = ui->createWalletPushButton->font();

    setScale();
    setStyle();
    ui->NewWalletFrame->repaint();
}

PageNewWallet::~PageNewWallet() {
    delete ui;
}

void PageNewWallet::open() {
    ui->walletNameLineEdit->clear();
    ui->walletPasswordLineEdit->clear();
    ui->walletPasswordLineEdit_2->clear();
}

void PageNewWallet::setScale() {
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    QRect rect = Global::Layout::getLayoutGeometryScaled();
    rect.setY(rect.y() + Global::Layout::getHeaderHeight());
    ui->NewWalletFrame->setGeometry(rect);

    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerLabel->setGeometry(Global::Layout::scaleRect(headerLabelQRectBack));
    ui->headerLabel->setFont(Global::Layout::scaleFont(headerLabelQFontBack));
    ui->walletNameLineEdit->setGeometry(Global::Layout::scaleRect(walletNameLineEditQRectBack));
    ui->walletNameLineEdit->setFont(Global::Layout::scaleFont(walletNameLineEditQFontBack));
    ui->walletPasswordLineEdit->setGeometry(Global::Layout::scaleRect(walletPasswordLineEditQRectBack));
    ui->walletPasswordLineEdit->setFont(Global::Layout::scaleFont(walletPasswordLineEditQFontBack));
    ui->walletPasswordLineEdit_2->setGeometry(Global::Layout::scaleRect(walletPasswordLineEdit2QRectBack));
    ui->walletPasswordLineEdit_2->setFont(Global::Layout::scaleFont(walletPasswordLineEdit2QFontBack));
    ui->showHidePushButton->setGeometry(Global::Layout::scaleRect(showHidePushButtonQRectBack));
    ui->showHidePushButton->setIconSize(Global::Layout::scaleSize(showHidePushButtonQSizeBack));
    ui->createWalletPushButton->setGeometry(Global::Layout::scaleRect(createWalletPushButtonQRectBack));
    ui->createWalletPushButton->setFont(Global::Layout::scaleFont(createWalletPushButtonQFontBack));
}

void PageNewWallet::setStyle() {
    Style::setButtonManagerPageStyle(ui->backPushButton);
    Style::setLabelStyle(ui->headerLabel);
    Style::setButtonManagerPageStyle(ui->showHidePushButton);
    Style::setLineEditManagerPageStyle(ui->walletNameLineEdit);
    Style::setLineEditManagerPageStyle(ui->walletPasswordLineEdit);
    Style::setLineEditManagerPageStyle(ui->walletPasswordLineEdit_2);
    Style::setButtonManagerPageStyle(ui->showHidePushButton);
    Style::setButtonManagerPageStyle(ui->createWalletPushButton);
}

void PageNewWallet::setVisibility(bool v) {
    ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
    ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    this->setVisible(v);
}

bool PageNewWallet::getVisibility() {
    return isVisible();
}

void PageNewWallet::on_showHidePushButton_clicked() {
    if (ui->walletPasswordLineEdit->echoMode() == QLineEdit::Password) {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordShown.png"));
    } else {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    }
}

void PageNewWallet::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::OPEN_WALLET);
}

void PageNewWallet::on_walletNameLineEdit_textChanged(const QString &arg1) {
    if(Global::Check::checkWalletFileNameValid(arg1))
        Style::setLineEditlWhiteStyle(ui->walletNameLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->walletNameLineEdit, "red");
    ui->walletNameLineEdit->style()->polish(ui->walletNameLineEdit);
}


void PageNewWallet::on_walletPasswordLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    if(Global::Check::password(ui->walletPasswordLineEdit->text()))
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit, "red");

    if(Global::Check::password(ui->walletPasswordLineEdit->text(), ui->walletPasswordLineEdit_2->text()))
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit_2);
    else
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit_2, "red");
    ui->walletPasswordLineEdit->style()->polish(ui->walletPasswordLineEdit);
}


void PageNewWallet::on_walletPasswordLineEdit_2_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    if(Global::Check::password(ui->walletPasswordLineEdit->text(), ui->walletPasswordLineEdit_2->text()))
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit_2);
    else
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit_2, "red");
    ui->walletPasswordLineEdit_2->style()->polish(ui->walletPasswordLineEdit_2);
}

void PageNewWallet::on_createWalletPushButton_clicked() {
    Global::Wallet::Name::set(ui->walletNameLineEdit->text());
    StorageCommon::storageError_e err = StorageInternal::walletCreate(ui->walletPasswordLineEdit->text());
    if(Global::Error::show(this, err))
        return;
    Global::Wallet::Password::set(ui->walletPasswordLineEdit->text());
    Global::Page::goManagerPage(Global::Page::NEW_ACCOUNT);
}


