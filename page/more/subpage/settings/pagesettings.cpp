#include "pagesettings.h"
#include "ui_pagesettings.h"

#include <QPixmap>
#include <QStandardItemModel>

#include "global.h"
#include "color.h"
#include "style.h"
#include "storage/storagesettings.h"

PageSettings::PageSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSettings) {
    ui->setupUi(this);

    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();

    settingsVerticalFrameQRectBack = ui->settingsVerticalFrame->geometry();

    languageComboBoxQRectBack = ui->languageComboBox->geometry();
    languageComboBoxQSizeBack = ui->languageComboBox->iconSize();
    languageComboBoxQFontBack = ui->languageComboBox->font();
    networkComboBoxQRectBack = ui->networkComboBox->geometry();
    networkComboBoxQSizeBack = ui->networkComboBox->iconSize();
    networkComboBoxQFontBack = ui->networkComboBox->font();
    backupWalletPushButtonQRectBack = ui->backupWalletPushButton->geometry();
    backupWalletPushButtonQSizeBack = ui->backupWalletPushButton->iconSize();
    backupWalletPushButtonQFontBack = ui->backupWalletPushButton->font();

    setStyle();
    setScale();

    ui->languageComboBox->setView(new QListView);
    ui->languageComboBox->addItem(QPixmap(":/flag/res/flag/us.png"), "ENG");
    ui->networkComboBox->setView(new QListView);
    ui->networkComboBox->addItems(networkNameList);
    /*QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->networkComboBox->model());
    if(model) {
        QStandardItem *item = model->item(2);
        item->setEnabled(false);
    }*/
    initialized = true;
}

PageSettings::~PageSettings() {
    delete ui;
}

void PageSettings::open() {
    initialized = false;
    ui->networkComboBox->setCurrentText(StorageSettings::get("network"));
    initialized = true;
}

void PageSettings::close() {

}

void PageSettings::setScale() {
    this->setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->settingsVerticalFrame->setGeometry(Global::Layout::scaleRect(settingsVerticalFrameQRectBack));

    //ui->languageComboBox->setGeometry(Global::Layout::scaleRect(languageComboBoxQRectBack));
    ui->languageComboBox->setIconSize(Global::Layout::scaleSize(languageComboBoxQSizeBack));
    ui->languageComboBox->setFont(Global::Layout::scaleFontOffset(languageComboBoxQFontBack));
    //ui->networkComboBox->setGeometry(Global::Layout::scaleRect(networkComboBoxQRectBack));
    ui->networkComboBox->setIconSize(Global::Layout::scaleSize(networkComboBoxQSizeBack));
    ui->networkComboBox->setFont(Global::Layout::scaleFontOffset(networkComboBoxQFontBack));
    //ui->backupWalletPushButton->setGeometry(Global::Layout::scaleRect(backupWalletPushButtonQRectBack));
    ui->backupWalletPushButton->setIconSize(Global::Layout::scaleSize(backupWalletPushButtonQSizeBack));
    ui->backupWalletPushButton->setFont(Global::Layout::scaleFontOffset(backupWalletPushButtonQFontBack));
}

void PageSettings::setStyle() {

}

void PageSettings::loop() {

}

void PageSettings::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::MORE);
}


void PageSettings::on_languageComboBox_currentTextChanged(const QString &arg1){
    if(!initialized)
        return;
    StorageSettings::set("language", arg1);
}


void PageSettings::on_networkComboBox_currentIndexChanged(int index) {
    if(!initialized)
        return;
    Global::Network::setNetwork((networkName_e)index);
    StorageSettings::set("network", Global::Network::getNetworkName((networkName_e)index));
    Global::Page::goManagerPage(Global::Page::ACCOUNT);
}

void PageSettings::on_backupWalletPushButton_clicked() {

}



