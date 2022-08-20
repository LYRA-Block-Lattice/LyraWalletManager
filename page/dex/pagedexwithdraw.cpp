#include "pagedexwithdraw.h"
#include "ui_pagedexwithdraw.h"

#include "global.h"
#include "color.h"
#include "style.h"

PageDexWithdraw::PageDexWithdraw(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDexWithdraw) {
    ui->setupUi(this);
}

PageDexWithdraw::~PageDexWithdraw() {
    delete ui;
}

void PageDexWithdraw::on_recipientAddressLineEdit_textChanged(const QString &arg1) {
    ui->recipientAddressLineEdit->style()->polish(ui->recipientAddressLineEdit);
    if(signatures::validateAccountId(arg1))
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit, "red");
}

void PageDexWithdraw::on_qrPushButton_clicked() {

}

void PageDexWithdraw::on_amountLineEdit_textChanged(const QString &arg1) {
    ui->amountLineEdit->style()->polish(ui->amountLineEdit);
    QString s = arg1;
    bool validate;
    s.remove(',').toDouble(&validate);
    if(s.length() > 0) {
        if(!validate) {
            ui->amountLineEdit->setText(Global::Util::normaliseNumber(s.left(s.length() - 1), false));
        } else {
            ui->amountLineEdit->setText(Global::Util::normaliseNumber(s.remove(','), false));
        }
    }
}

void PageDexWithdraw::on_maximumPushButton_clicked() {

}

void PageDexWithdraw::on_withdrawPushButton_clicked() {

}

