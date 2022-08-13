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
            bool Valid = false;
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
        bool valid = false;
    };
    class Pool {
    public:
        Pool(QString data);
        ~Pool();
        qint64 getHeight() { return height;}
        QString getPoolId() { return poolId;};
        QString getToken0() { return token0;};
        QString getToken1() { return token1;};
        double getBalanceToken0() { return balanceToken0;};
        double getBalanceToken1() { return balanceToken1;};
        bool getValid() { return valid;};
    private:
        qint64 height = 0;
        QString poolId;
        QString token0;
        QString token1;
        double balanceToken0 = 0.0;
        double balanceToken1 = 0.0;
        bool valid = false;
    };
    class PoolCalculate {
    public:
        PoolCalculate(QString data);
        ~PoolCalculate();
        qint64 getHeight() { return height;}
        double getProviderFee() { return providerFee;}
        double getProtocolFee() { return protocolFee;}
        QString getSwapInToken() { return swapInToken;}
        double getSwapInAmount() { return swapInAmount;}
        QString getSwapOutToken() { return swapOutToken;}
        double getSwapOutAmount() { return swapOutAmount;}
        double getPrice() { return price;}
        double getPriceImpact() { return priceImpact;}
        double getMinimumReceived() { return minimumReceived;}
        double getPayToProvider() { return payToProvider;}
        double getPayToAuthorizer() { return payToAuthorizer;}
        bool getValid() { return valid;}
    private:
        qint64 height = 0;
        double providerFee = 0.0;
        double protocolFee = 0.0;
        QString swapInToken;
        double swapInAmount = 0.0;
        QString swapOutToken;
        double swapOutAmount = 0.0;
        double price = 0.0;
        double priceImpact = 0.0;
        double minimumReceived = 0.0;
        double payToProvider = 0.0;
        double payToAuthorizer = 0.0;
        bool valid = false;
    };
};

#endif // RPCCLASS_H
