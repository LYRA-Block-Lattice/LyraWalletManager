#include "pagerecoveraccount.h"
#include "ui_pagerecoveraccount.h"

#include "style.h"
#include "global.h"

PageRecoverAccount::PageRecoverAccount(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageRecoverAccount) {
    ui->setupUi(this);
    ui->RecoverAccountFrame->setStyleSheet("background-color: #FCD535;");

    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerLabelQRectBack = ui->headerLabel->geometry();
    headerLabelQFontBack = ui->headerLabel->font();
    accountNameLineEditQRectBack = ui->accountNameLineEdit->geometry();
    accountNameLineEditQFontBack = ui->accountNameLineEdit->font();
    walletPasswordLineEditQRectBack = ui->walletPasswordLineEdit->geometry();
    walletPasswordLineEditQFontBack = ui->walletPasswordLineEdit->font();
    privateKeyLineEditQRectBack = ui->privateKeyLineEdit->geometry();
    privateKeyLineEditQFontBack = ui->privateKeyLineEdit->font();
    showHidePushButtonQRectBack = ui->showHidePushButton->geometry();
    showHidePushButtonQSizeBack = ui->showHidePushButton->iconSize();
    recoverAccountPushButtonQRectBack = ui->recoverAccountPushButton->geometry();
    recoverAccountPushButtonQFontBack = ui->recoverAccountPushButton->font();

    setScale();
    setStyle();
    ui->RecoverAccountFrame->repaint();
}

PageRecoverAccount::~PageRecoverAccount() {
    delete ui;
}

void PageRecoverAccount::open() {
    ui->accountNameLineEdit->clear();
    ui->walletPasswordLineEdit->clear();
    ui->privateKeyLineEdit->clear();
}

void PageRecoverAccount::setScale() {
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());
    QRect rect = Global::Layout::getLayoutGeometryScaled();
    rect.setY(rect.y() + Global::Layout::getHeaderHeight());
    ui->RecoverAccountFrame->setGeometry(rect);

    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerLabel->setGeometry(Global::Layout::scaleRect(headerLabelQRectBack));
    ui->headerLabel->setFont(Global::Layout::scaleFont(headerLabelQFontBack));
    ui->accountNameLineEdit->setGeometry(Global::Layout::scaleRect(accountNameLineEditQRectBack));
    ui->accountNameLineEdit->setFont(Global::Layout::scaleFont(accountNameLineEditQFontBack));
    ui->walletPasswordLineEdit->setGeometry(Global::Layout::scaleRect(walletPasswordLineEditQRectBack));
    ui->walletPasswordLineEdit->setFont(Global::Layout::scaleFont(walletPasswordLineEditQFontBack));
    ui->privateKeyLineEdit->setGeometry(Global::Layout::scaleRect(privateKeyLineEditQRectBack));
    ui->privateKeyLineEdit->setFont(Global::Layout::scaleFont(privateKeyLineEditQFontBack));
    ui->showHidePushButton->setGeometry(Global::Layout::scaleRect(showHidePushButtonQRectBack));
    ui->showHidePushButton->setIconSize(Global::Layout::scaleSize(showHidePushButtonQSizeBack));
    ui->recoverAccountPushButton->setGeometry(Global::Layout::scaleRect(recoverAccountPushButtonQRectBack));
    ui->recoverAccountPushButton->setFont(Global::Layout::scaleFont(recoverAccountPushButtonQFontBack));
}

void PageRecoverAccount::setStyle() {
    Style::setButtonManagerPageStyle(ui->backPushButton);
    Style::setLabelStyle(ui->headerLabel);
    Style::setButtonManagerPageStyle(ui->showHidePushButton);
    Style::setLineEditManagerPageStyle(ui->accountNameLineEdit);
    Style::setLineEditManagerPageStyle(ui->walletPasswordLineEdit);
    Style::setLineEditManagerPageStyle(ui->privateKeyLineEdit);
    Style::setButtonManagerPageStyle(ui->showHidePushButton);
    Style::setButtonManagerPageStyle(ui->recoverAccountPushButton);
}

void PageRecoverAccount::setVisibility(bool v) {
    ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
    ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    this->setVisible(v);
}

bool PageRecoverAccount::getVisibility() {
    return isVisible();
}

void PageRecoverAccount::on_showHidePushButton_clicked() {
    if (ui->walletPasswordLineEdit->echoMode() == QLineEdit::Password) {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Normal);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordShown.png"));
    } else {
        ui->walletPasswordLineEdit->setEchoMode(QLineEdit::Password);
        ui->showHidePushButton->setIcon(QIcon(":/res/ic/res/ic/passwordHidden.png"));
    }
}

void PageRecoverAccount::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::NEW_ACCOUNT);
}

void PageRecoverAccount::on_recoverAccountPushButton_clicked() {
    StorageCommon::storageError_e err = StorageInternal::accountCreate(ui->accountNameLineEdit->text(), ui->walletPasswordLineEdit->text(),ui->privateKeyLineEdit->text());
    if(Global::Error::show(this, err))
        return;
    Global::Page::goManagerPage(Global::Page::ACCOUNT);
}


void PageRecoverAccount::on_accountNameLineEdit_textChanged(const QString &arg1) {
    ui->accountNameLineEdit->style()->polish(ui->accountNameLineEdit);
    if(Global::Check::checkWalletFileNameValid(arg1))
        Style::setLineEditlWhiteStyle(ui->accountNameLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->accountNameLineEdit, "red");
}


void PageRecoverAccount::on_walletPasswordLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->walletPasswordLineEdit->style()->polish(ui->walletPasswordLineEdit);
    if(Global::Check::password(ui->walletPasswordLineEdit->text()))
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->walletPasswordLineEdit, "red");
}


void PageRecoverAccount::on_privateKeyLineEdit_textChanged(const QString &arg1) {
    Q_UNUSED(arg1)
    ui->privateKeyLineEdit->style()->polish(ui->privateKeyLineEdit);
    if(signatures::validatePrivateKey(ui->privateKeyLineEdit->text()))
        Style::setLineEditlWhiteStyle(ui->privateKeyLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->privateKeyLineEdit, "red");
}

