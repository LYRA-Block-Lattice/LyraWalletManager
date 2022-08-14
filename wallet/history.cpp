#include "history.h"

#include <QList>

#include "global.h"

QList<Wallet::History::HistoryEntry_t> HistoryList = QList<Wallet::History::HistoryEntry_t>();
int changeCount = 0;

Wallet::History::HistoryEntry_e Wallet::History::set(QString data) {
    if(Global::Account::getSelectedAccountName().length() == 0)
        return HistoryEntry_e::ERR_INVALID_ACCOUNT;
    RpcClass::History *history = new RpcClass::History(data);
    if(!history->getValid()) {
        delete history;
        return HistoryEntry_e::ERR_DECODE;
    }
    for (int i =0; i < HistoryList.count(); i++) {
        if(!HistoryList[i].Name.compare(Global::Util::getAccountNameAndNetwork())) {
            delete HistoryList[i].history;
            HistoryList[i].history = history;
            HistoryList[i].ChangeCount++;
            changeCount++;
            return HistoryEntry_e::ERR_OK;
        }
    }
    HistoryEntry_t entry;
    entry.ChangeCount = 0;
    entry.Name = Global::Util::getAccountNameAndNetwork();
    entry.history = history;
    HistoryList.append(entry);
    changeCount++;
    return HistoryEntry_e::ERR_OK;
}

Wallet::History::HistoryEntry_e Wallet::History::set(RpcClass::History *history) {
    if(Global::Account::getSelectedAccountName().length() == 0)
        return HistoryEntry_e::ERR_INVALID_ACCOUNT;
    if(!history->getValid()) {
        delete history;
        return HistoryEntry_e::ERR_DECODE;
    }
    for (int i =0; i < HistoryList.count(); i++) {
        if(!HistoryList[i].Name.compare(Global::Util::getAccountNameAndNetwork())) {
            delete HistoryList[i].history;
            HistoryList[i].history = history;
            HistoryList[i].ChangeCount++;
            changeCount++;
            return HistoryEntry_e::ERR_OK;
        }
    }
    HistoryEntry_t entry;
    entry.ChangeCount = 0;
    entry.Name = Global::Util::getAccountNameAndNetwork();
    entry.history = history;
    HistoryList.append(entry);
    changeCount++;
    return HistoryEntry_e::ERR_OK;
}

Wallet::History::HistoryEntry_t *Wallet::History::get() {
    for (int i =0; i < HistoryList.count(); i++) {
        if(!HistoryList[i].Name.compare(Global::Util::getAccountNameAndNetwork())) {
            return &HistoryList[i];
        }
    }
    return nullptr;
}

double Wallet::History::getTickerBalance(QString ticker) {
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        QList<RpcClass::History::entry_t> tmpListEntrys = historyInst->getHistory();
        QList<QPair<QString, double>> balances = tmpListEntrys.last().Balances;
        QPair<QString, double> balance;
        foreach(balance, balances) {
            if(!balance.first.compare(ticker))
                return balance.second;
        }
    }
    return 0.0;
}

QList<QString> Wallet::History::getAvailableTikers() {
    Wallet::History::HistoryEntry_t *historyEntry = Wallet::History::get();
    QList<QString> tickers;
    if(historyEntry) {
        RpcClass::History *historyInst = historyEntry->history;
        RpcClass::History::entry_t hEntry;
        QList<RpcClass::History::entry_t> tmpListEntrys = historyInst->getHistory();
        QList<QPair<QString, double>> balances = tmpListEntrys.last().Balances;
        QPair<QString, double> balance;
        foreach(balance, balances) {
            tickers.append(balance.first);
        }
    }
    return tickers;
}

int Wallet::History::getChangeCount() {
    return changeCount;
}
