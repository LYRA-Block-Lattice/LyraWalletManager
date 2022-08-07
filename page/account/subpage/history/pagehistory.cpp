#include "pagehistory.h"
#include "ui_pagehistory.h"

#include <QListWidgetItem>
#include <QListView>
#include <QVBoxLayout>

#include "style.h"
#include "global.h"

#include "wallet/history.h"

PageHistory::PageHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageHistory) {
    this->parent = parent;
    //setGeometry(Global::Layout::getTabLayoutGeometryScaled());
    ui->setupUi(this);
    setStyle();

    // Backup items geometry, size and font.
    //ui->headerFrame->setParent(this);
    headerFrameQRectBack = ui->headerFrame->geometry();
    titleLabelQRectBack = ui->titleLabel->geometry();
    titleLabelQFontBack = ui->titleLabel->font();
    backPushButtonQRectBack = ui->backPushButton->geometry();
    backPushButtonQSizeBack = ui->backPushButton->iconSize();


    //messageList->setGeometry(QRect(0, Global::Layout::scaleValueX(62), geometry().width(), geometry().height() - Global::Layout::scaleValueX(62) - Global::Layout::scaleValueX(MENU_BAR_HEIGHT + 10)));

    messageList = new PageHistoryList(this);
    messageListQRectBack = messageList->geometry();

    historyLoadProgressBar = new QProgressBar(this);
    historyLoadProgressBar->setGeometry(0, 0, 311, 5);
    historyLoadProgressBar->setTextVisible(false);
    historyLoadProgressBarQRectBack = historyLoadProgressBar->geometry();
    setScale();


    messageList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    messageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    messageList->scrollToTop();
    messageList->setEnabled(true);
    //setGeometry(Global::Layout::getTabLayoutGeometryScaled());

    connect(messageList, &PageHistoryList::itemClicked, this, &PageHistory::on_ItemSelected);

}

PageHistory::~PageHistory() {
    delete ui;
}

void PageHistory::open() {
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    QString k;
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        //QList<QPair<QString, double>> changes;
        tmpListEntrys = historyInst->getHistory();
        std::reverse(std::begin(tmpListEntrys), std::end(tmpListEntrys));
        foreach(hEntry, tmpListEntrys) {
            k += QString::number(hEntry.TimeStamp);
        }
        if(listHash.compare(k)) {
            showProgress(true);
            historyLoadProgressBar->setMaximum(tmpListEntrys.count());
            messageList->clearAll();
            int itemsCount = 0;
            listHash.clear();
            foreach(hEntry, tmpListEntrys) {
                listHash += QString::number(hEntry.TimeStamp);
                if( itemsCount % 20 == 9 )
                    QApplication::processEvents();
                messageList->addMessage(QPixmap(Global::TickerIcon::get(hEntry.Changes[0].first)),
                                        hEntry.Changes[0].first, hEntry.Changes[0].second,
                                        hEntry.Changes[0].second * Global::TickerPrice::get(hEntry.Changes[0].first),
                                        Global::TickerPrice::get(hEntry.Changes[0].first),
                                        hEntry.Height);
                messageList->scrollToTop();
                itemsCount++;
                historyLoadProgressBar->setValue(itemsCount);
            }
        }
        showProgress(false);
    }
}

void PageHistory::close() {

}

void PageHistory::setScale() {
    setGeometry(Global::Layout::getLayoutGeometryScaled());
    ui->backPushButton->setGeometry(Global::Layout::scaleRect(backPushButtonQRectBack));
    ui->backPushButton->setIconSize(Global::Layout::scaleSize(backPushButtonQSizeBack));
    ui->headerFrame->setGeometry(Global::Layout::scaleRect(headerFrameQRectBack));
    ui->titleLabel->setGeometry(Global::Layout::scaleRect(titleLabelQRectBack));
    //progressLabel->setGeometry(Global::Layout::scaleRect(progressMovieQRectBack));
    messageList->setGeometry(QRect(0, Global::Layout::scaleValueX(62), geometry().width(), Global::Layout::getLayoutGeometryScaled().height() - Global::Layout::scaleValueX(62)/* - Global::Layout::scaleValueX(MENU_BAR_HEIGHT + 10)*/));
    historyLoadProgressBar->setGeometry(Global::Layout::scaleRect(historyLoadProgressBarQRectBack));
}

void PageHistory::setStyle() {

}

void PageHistory::loop() {
    if(Wallet::History::get()) {
        if(AccountListChangedCount != Global::Account::getAccountListChangedCount() ||
                historyChangeCount != Wallet::History::getChangeCount()) {
            AccountListChangedCount = Global::Account::getAccountListChangedCount();
            historyChangeCount = Wallet::History::getChangeCount();
            open();
        }
    }
}

void PageHistory::showProgress(bool show) {
    if(show) {
        ui->backPushButton->setEnabled(false);
        messageList->setEnabled(false);
        historyLoadProgressBar->setVisible(true);
    } else {
        ui->backPushButton->setEnabled(true);
        messageList->setEnabled(true);
        historyLoadProgressBar->setVisible(false);
    }
}

void PageHistory::on_ItemSelected(int item) {
    Global::Page::goManagerPage(Global::Page::TRANSACTION_DETAIL, &tmpListEntrys[item]);
}

void PageHistory::on_backPushButton_clicked() {
    Global::Page::goManagerPage(Global::Page::ACCOUNT);
    close();
}

