#include "pagenewaccount.h"
#include "ui_pagenewaccount.h"

#include <QMessageBox>

#include "style.h"
#include "global.h"

PageNewAccount::PageNewAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageNewAccount) {
    ui->setupUi(this);
    ui->NewAccountFrame->setStyleSheet("background-color: #FCD535;");

    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerLabelQRectBack = ui->headerLabel->geometry();
    headerLabelQFontBack = ui->headerLabel->font();
    accountNameLineEditQRectBack = ui->accountNameLineEdit->geometry();
    accountNameLineEditQFontBack = ui->accountNameLineEdit->font();
    walletPasswordLineEditQRectBack = ui->walletPasswordLineEdit->geometry();
    walletPasswordLineEditQFontBack = ui->walletPasswordLineEdit->font();
    showHidePushButtonQRectBack = ui->showHidePushButton->geometry();
    showHidePushButtonQSizeBack = ui->showHidePushButton->iconSize();
    createAccountPushButtonQRectBack = ui->createAccountPushButton->geometry();
    createAccountPushButtonQFontBack = ui->createAccountPushButton->font();
    recoverAccountPushButtonQRectBack = ui->recoverAccountPushButton->geometry();
    recoverAccountPushButtonQFontBack = ui->recoverAccountPushButton->font();

    setScale();
    setStyle();
    ui->NewAccountFrame->repaint();
}

PageNewAccount::~PageNewAccount() {
    delete ui;
}

void PageNewAccount::open() {
    ui->accountNameLineEdit->clear();
    ui->walletPasswordLineEdit->clear();
    if(Global::Account::getAccountList().count())
        ui->backPushButton->setVisible(true);
    else
        ui->backPushButton->setVisible(false);
}

void PageNewAccount::setScale() {
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    QRect rect = Global::Layout::getLayoutGeometryScaled();
    rect.setY(rect.y() + Global::Layout::getHeaderHeight());
    ui->NewAccountFrame->setGeometry(rect);

    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerLabel->setGeometry(Global::Layout::scaleRect(headerLabelQRectBack));
    ui->headerLabel->setFont(Global::Layout::scaleFont(headerLabelQFontBack));
    ui->accountNameLineEdit->setGeometry(Global::Layout::scaleRect(accountNameLineEditQRectBack));
    ui->accountNameLineEdit->setFont(Global::Layout::scaleFont(accountNameLineEditQFontBack));
    ui->walletPasswordLineEdit->setGeometry(Global::Layout::scaleRect(walletPasswordLineEditQRectBack));
    ui->walletPasswordLineEdit->setFont(Global::Layout::scaleFont(walletPasswordLineEditQFontBack));
    ui->showHidePushButton->setGeometry(Global::Layout::scaleRect(showHidePushButtonQRectBack));
    ui->showHidePushButton->setIconSize(Global::Layout::scaleSize(showHidePushButtonQSizeBack));
    ui->createAccountPushButton->setGeometry(Global::Layout::scaleRect(createAccountPushButtonQRectBack));
    ui->createAccountPushButton->setFont(Global::Layout::scaleFont(createAccountPushButtonQFontBack));
    ui->recoverAccountPushButton->setGeometry(Global::Layout::scaleRect(recoverAccountPushButtonQRectBack));
    ui->recoverAccountPushButton->setFont(Global::Layout::scaleFont(recoverAccountPushButtonQFontBack));
}

void PageNewAccount::setStyle() {
    /*Style::setButtonStyle(ui->backPushButton);
    Style::setLabelStyle(ui->headerLabel);
    Style::setLineEditlStyle(ui->accountNameLineEdit);
    Style::setLineEditlStyle(ui->walletPasswordLineEdit);
    Style::setButtonStyle(ui->showHidePushButton);
    Style::setButtonStyle(ui->createAccountPushButton);
    Style::setButtonStyle(ui->recoverAccountPushButton);*/
}


bool PageNewAccount::getVisibility() {
    ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
    ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    return isVisible();
}

void PageNewAccount::on_showHidePushButton_clicked() {
    if (ui->walletPasswordLineEdit->echoMode() == QLineEdit::Password) {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordShown.png"));
    } else {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    }
}

void PageNewAccount::setVisibility(bool v) {
    this->setVisible(v);
}

void PageNewAccount::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::ACCOUNT);
}


void PageNewAccount::on_createAccountPushButton_clicked() {
    StorageCommon::storageError_e err = StorageInternal::accountCreate(ui->accountNameLineEdit->text(), ui->walletPasswordLineEdit->text());
    if(Global::Error::show(this, err))
        return;
    Global::Page::goManagerPage(Global::Page::ACCOUNT);
}


void PageNewAccount::on_recoverAccountPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::RECOVER_ACCOUNT);
}


void PageNewAccount::on_accountNameLineEdit_textChanged(const QString &arg1) {
    ui->accountNameLineEdit->style()->polish(ui->accountNameLineEdit);
    if(Global::Check::checkFileNameWithSpacesValid(arg1))
        Style::setLineEditlWhiteStyle(ui->accountNameLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->accountNameLineEdit, "red");
}


void PageNewAccount::on_walletPasswordLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->walletPasswordLineEdit->style()->polish(ui->walletPasswordLineEdit);
    if(Global::Check::password(ui->walletPasswordLineEdit->text()))
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit, "red");
}

