#ifndef RPCCLASS_H
#define RPCCLASS_H

#include <QString>
#include <QList>
#include <QPair>

#include "globallyra.h"

namespace RpcClass{
    class History {
    public:
        History(QString data);
        History(RpcClass::History *history);
        ~History();
        typedef struct entry_s{
            long long Height;
            bool IsReceive;
            long long TimeStamp;
            QString SendAccountId;
            QString SendHash;
            QString RecvAccountId;
            QString RecvHash;
            QList<QPair<QString, double>> Changes;
            QList<QPair<QString, double>> Balances;
        }entry_t;

        struct {
            networkName_e NetworkName;
            QString AccountName;
            QList<entry_t> History;
            QString HistoryData;
            bool Valid;
        }Account;

        networkName_e getNetwork() { return Account.NetworkName; }
        QString getAccountName() { return Account.AccountName; }
        QList<entry_t> getHistory() { return Account.History; }
        QString getData() { return Account.HistoryData; }
        bool getValid() { return Account.Valid; }
    };
    class Balance {
    public:
        Balance(QString data);
        ~Balance();
        QList<QPair<QString, double>> getBalances() { return Balances; }
        qint64 getHeight() { return height; }
        bool getUnreceived() { return unreceived; }
        bool getValid() { return valid; }
    private:
        QList<QPair<QString, double>> Balances;
        qint64 height = 0;
        bool unreceived = false;
        bool valid;
    };
};

#endif // RPCCLASS_H
