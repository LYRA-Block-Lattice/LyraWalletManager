#include "pagesend.h"
#include "ui_pagesend.h"

#include <QListView>
#include <QDoubleValidator>
#include <QCamera>
#include <QMediaDevices>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QMessageBox>
#include <QGraphicsOpacityEffect>

#include "style.h"
#include "global.h"
#include "globallyra.h"
#include "crypto/signatures.h"
#include "wallet/history.h"
#include "translate/translate.h"


static QTimer fadeTimer;
static int fadeCount;

PageSend::PageSend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSend) {
    ui->setupUi(this);

    ui->sendingLabel->setVisible(false);
    fadeCount = 0;

    // Backup items geometry, size and font.
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();

    tokenComboBoxQRectBack = ui->tokenComboBox->geometry();
    tokenComboBoxQSizeBack = ui->tokenComboBox->iconSize();
    tokenComboBoxQFontBack = ui->tokenComboBox->font();
    recipientAddressLineEditQRectBack = ui->recipientAddressLineEdit->geometry();
    recipientAddressLineEditQFontBack = ui->recipientAddressLineEdit->font();
    amountLineEditQRectBack = ui->amountLineEdit->geometry();
    amountLineEditQFontBack = ui->amountLineEdit->font();

    qrPushButtonQRectBack = ui->qrPushButton->geometry();
    qrPushButtonQSizeBack = ui->qrPushButton->iconSize();
    maximumPushButtonQRectBack = ui->maximumPushButton->geometry();
    maximumPushButtonQSizeBack = ui->maximumPushButton->iconSize();

    sendPushButtonQRectBack = ui->sendPushButton->geometry();
    sendPushButtonQFontBack= ui->sendPushButton->font();

    sendingLabelQRectBack = ui->sendingLabel->geometry();
    sendingLabelQFontBack = ui->sendingLabel->font();

    ui->tokenComboBox->setIconSize(QSize(20, 20));
    ui->tokenComboBox->setView(new QListView);
    QListView *list = (QListView *)ui->tokenComboBox->view();
    list->setIconSize(QSize(20, 20));
    //ui->tokenComboBox->addItem(QPixmap(), "");
    setStyle();
    setScale();

    //ui->amountLineEdit->setValidator(new QDoubleValidator(0, 10000000000, 8));


    fadeTimer.setInterval(20);
    fadeTimer.stop();
    connect(&fadeTimer, &QTimer::timeout, this, [=] {
        if(fadeCount == FADE_COUNT_START_VALE)
            ui->sendingLabel->setVisible(true);
        double opacity = 0;
        if(fadeCount == 0) {
            opacity = 0.0;
            ui->sendingLabel->setVisible(false);
            fadeTimer.stop();
        } else  if(fadeCount < 100) {
            opacity = (double)fadeCount / 100.0;
            fadeCount--;
        } else {
            opacity = 1.0;
            fadeCount--;
        }
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(opacity);
        ui->sendingLabel->setGraphicsEffect(effect);
    });
}

PageSend::~PageSend() {
    delete ui;
}

void PageSend::open() {
    camera = nullptr;
    fadeCount = 0;
    ui->sendingLabel->setVisible(false);
    ui->amountLineEdit->clear();
    ui->recipientAddressLineEdit->clear();
    ui->tokenComboBox->clear();
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        QList<RpcClass::History::entry_t> tmpListEntrys = historyInst->getHistory();
        QList<QPair<QString, double>> balances = tmpListEntrys.last().Balances;
        QPair<QString, double> balance;
        foreach(balance, balances) {
            ui->tokenComboBox->addItem(QPixmap(Global::TickerIcon::get(balance.first)), balance.first);
        }
    }
}

void PageSend::close() {
    camera = nullptr;
}

void PageSend::setScale() {
    double scale = Global::Layout::labelFontScale();
    this->setGeometry(Global::Layout::getLayoutGeometryScaled());

    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    ui->titleLabel->setFont(Global::Layout::scaleFontOffset(titleLabelQFontBack));

    ui->tokenComboBox->setGeometry(Global::Layout::scaleRect(tokenComboBoxQRectBack));;
    ui->tokenComboBox->setFont(Global::Layout::scaleFontOffset(tokenComboBoxQFontBack));;
    ui->tokenComboBox->setIconSize(Global::Layout::scaleSize(tokenComboBoxQSizeBack));
    QListView *list = (QListView *)ui->tokenComboBox->view();
    list->setIconSize(Global::Layout::scaleSize(tokenComboBoxQSizeBack));
    list->update();
    ui->recipientAddressLineEdit->setGeometry(Global::Layout::scaleRect(recipientAddressLineEditQRectBack));;
    ui->recipientAddressLineEdit->setFont(Global::Layout::scaleFontOffset(recipientAddressLineEditQFontBack, scale));
    ui->amountLineEdit->setGeometry(Global::Layout::scaleRect(amountLineEditQRectBack));;
    ui->amountLineEdit->setFont(Global::Layout::scaleFontOffset(amountLineEditQFontBack, scale));

    ui->qrPushButton->setGeometry(Global::Layout::scaleRect(qrPushButtonQRectBack));
    ui->qrPushButton->setIconSize(Global::Layout::scaleSize(qrPushButtonQSizeBack));
    ui->maximumPushButton->setGeometry(Global::Layout::scaleRect(maximumPushButtonQRectBack));
    ui->maximumPushButton->setIconSize(Global::Layout::scaleSize(maximumPushButtonQSizeBack));

    ui->sendPushButton->setGeometry(Global::Layout::scaleRect(sendPushButtonQRectBack));
    ui->sendPushButton->setFont(Global::Layout::scaleFontOffset(sendPushButtonQFontBack));

    ui->sendingLabel->setGeometry(Global::Layout::scaleRect(sendingLabelQRectBack));;
    ui->sendingLabel->setFont(Global::Layout::scaleFontOffset(sendingLabelQFontBack));

}

void PageSend::setStyle() {
    Style::setButtontOnHeaderStyle(ui->backPushButton);
    Style::setComboBoxStyle(ui->tokenComboBox, tokenComboBoxQFontBack);
    Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit);
    Style::setLineEditlWhiteStyle(ui->amountLineEdit);
    Style::setButtonDefaultStyle(ui->sendPushButton);
    Style::setButtonTransparentStyle(ui->qrPushButton);
    Style::setButtonTransparentStyle(ui->maximumPushButton);
}

void PageSend::loop() {

}

bool PageSend::checkCameraAvailability() {
    if (QMediaDevices::videoInputs().count() > 0)
        return true;
    else
        return false;
}

void PageSend::checkIntegrityOfInputs() {
    if(!signatures::validateAccountId(ui->recipientAddressLineEdit->text())) {
        ui->sendPushButton->setEnabled(false);
        return;
    }
    bool validate;
    ui->amountLineEdit->text().remove(',').toDouble(&validate);
    if(!validate) {
        ui->sendPushButton->setEnabled(false);
        return;
    }
    ui->sendPushButton->setEnabled(true);
}

bool PageSend::checkEnoughFunds() {
    bool validate;
    double amount = ui->amountLineEdit->text().remove(',').toDouble(&validate);
    if(!validate) {
        ui->sendingLabel->setText(Tr("Invalid amount"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return false;
    }
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        QList<RpcClass::History::entry_t> tmpListEntrys = historyInst->getHistory();
        QList<QPair<QString, double>> balances = tmpListEntrys.last().Balances;
        double lyrAmount = 0.0;
        QPair<QString, double> balance;
        foreach(balance, balances) {
            if(!balance.first.compare("LYR")) {
                lyrAmount = balance.second;
            }
        }
        if(lyrAmount < LYRA_TX_FEE) {
            ui->sendingLabel->setText(Tr("Not enough funds for fee"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            return false;
        }
        foreach(balance, balances) {
            if(!balance.first.compare(ui->tokenComboBox->currentText())) {
                if(!balance.first.compare("LYR")) {
                    if(balance.second - LYRA_TX_FEE >= amount) {
                        return true;
                    }
                } else {
                    if(balance.second >= amount) {
                        return true;
                    }
                }
            }
        }
        ui->sendingLabel->setText(Tr("Not enough funds"));
        fadeCount = FADE_COUNT_START_VALE;
        fadeTimer.start();
        return false;
    }
    ui->sendingLabel->setText(Tr("Account is empty"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    return false;
}

void PageSend::on_backPushButton_clicked() {
    /*if( camera )
        if( camera->isVisible())
            camera->close();*/
    Global::Page::goManagerPage(Global::Page::ACCOUNT);
    close();
}

void PageSend::on_recipientAddressLineEdit_textChanged(const QString &arg1) {
    ui->recipientAddressLineEdit->style()->polish(ui->recipientAddressLineEdit);
    if(signatures::validateAccountId(arg1))
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit);
    else
        Style::setLineEditlWhiteStyle(ui->recipientAddressLineEdit, "red");
    checkIntegrityOfInputs();
}


void PageSend::on_amountLineEdit_textChanged(const QString &arg1) {
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
    checkIntegrityOfInputs();
}


void PageSend::on_qrPushButton_clicked() {
    if(checkCameraAvailability()) {
        camera = new Camera(this);
        connect(camera, &Camera::qrRead, this, [=](QString qr) {
            ui->recipientAddressLineEdit->setText(qr);
            ui->qrPushButton->setEnabled(true);
        });
        connect(camera, &Camera::windowClosed, this, [=]{
            ui->qrPushButton->setEnabled(true);
        });
        camera->show();
        ui->qrPushButton->setEnabled(false);
    } else {
        QMessageBox::critical(this, "ERROR", Tr("No camera device detected"));
    }
}

void PageSend::on_sendPushButton_clicked() {
    if(!checkEnoughFunds()) {
        return;
    }
    QString sendreview = Tr("Do you want to send") + ":\n" +
            ui->amountLineEdit->text() + " " + ui->tokenComboBox->currentText() + "\n" +
            Tr("to address") + "\n" +
            Global::Util::truncateIdHash(ui->recipientAddressLineEdit->text(), 23);
    QMessageBox* const message = new QMessageBox(QMessageBox::Icon::Question, tr("Send") + "?",
        sendreview, QMessageBox::Button::Yes | QMessageBox::Button::No, this);
    message->setDefaultButton(QMessageBox::Button::No);
    message->open();
    connect(message, &QDialog::finished, this, [message, this] {
        message->deleteLater();
        if (message->result() == QMessageBox::Button::Yes) {
            ui->sendingLabel->setText(Tr("Sending"));
            fadeCount = FADE_COUNT_START_VALE;
            fadeTimer.start();
            sendThread = new WalletRpc::Send;
            sendWorkerThread = new QThread;
            sendThread->moveToThread(sendWorkerThread);
            connect(sendWorkerThread, &QThread::finished, sendThread, &QObject::deleteLater);
            connect(this, &PageSend::sendOperate, sendThread, &WalletRpc::Send::doWork);
            connect(sendThread, &WalletRpc::Send::resultReady, this, &PageSend::on_SendRetriveDone);
            connect(sendThread, &WalletRpc::Send::resultError, this, &PageSend::on_SendRetriveError);
            sendWorkerThread->start();
            emit sendOperate(ui->amountLineEdit->text().remove(','), ui->recipientAddressLineEdit->text(), Global::Util::signToTicker(ui->tokenComboBox->currentText()));
        }
    });
}

void PageSend::on_SendRetriveDone(const QString &s) {
    Q_UNUSED(s)
    ui->sendingLabel->setText(Tr("Sent"));
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
    ui->amountLineEdit->setText("");
    ui->recipientAddressLineEdit->setText("");
}

void PageSend::on_SendRetriveError(const QString &s) {
    ui->sendingLabel->setText(s);
    fadeCount = FADE_COUNT_START_VALE;
    fadeTimer.start();
}

void PageSend::on_maximumPushButton_clicked() {

}

