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

int Wallet::History::getChangeCount() {
    return changeCount;
}
