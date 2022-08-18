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
        qint64 getHeight() { return Height; }
        bool getUnreceived() { return Unreceived; }
        bool getValid() { return Valid; }
    private:
        QList<QPair<QString, double>> Balances;
        qint64 Height = 0;
        bool Unreceived = false;
        bool Valid = false;
    };
    class Pool {
    public:
        Pool(QString data);
        ~Pool();
        qint64 getHeight() { return Height;}
        QString getPoolId() { return PoolId;};
        QString getToken0() { return Token0;};
        QString getToken1() { return Token1;};
        double getBalanceToken0() { return BalanceToken0;};
        double getBalanceToken1() { return BalanceToken1;};
        bool getValid() { return Valid;};
    private:
        qint64 Height = 0;
        QString PoolId;
        QString Token0;
        QString Token1;
        double BalanceToken0 = 0.0;
        double BalanceToken1 = 0.0;
        bool Valid = false;
    };
    class PoolCalculate {
    public:
        PoolCalculate(QString data);
        ~PoolCalculate();
        qint64 getHeight() { return Height;}
        double getProviderFee() { return ProviderFee;}
        double getProtocolFee() { return ProtocolFee;}
        QString getSwapInToken() { return SwapInToken;}
        double getSwapInAmount() { return SwapInAmount;}
        QString getSwapOutToken() { return SwapOutToken;}
        double getSwapOutAmount() { return SwapOutAmount;}
        double getPrice() { return Price;}
        double getPriceImpact() { return PriceImpact;}
        double getMinimumReceived() { return MinimumReceived;}
        double getPayToProvider() { return PayToProvider;}
        double getPayToAuthorizer() { return PayToAuthorizer;}
        bool getValid() { return Valid;}
    private:
        qint64 Height = 0;
        double ProviderFee = 0.0;
        double ProtocolFee = 0.0;
        QString SwapInToken;
        double SwapInAmount = 0.0;
        QString SwapOutToken;
        double SwapOutAmount = 0.0;
        double Price = 0.0;
        double PriceImpact = 0.0;
        double MinimumReceived = 0.0;
        double PayToProvider = 0.0;
        double PayToAuthorizer = 0.0;
        bool Valid = false;
    };

    class CreatePool {
    public:
        CreatePool(QString data);
        ~CreatePool();
        qint64 getHeight() { return Height; }
        QString getPoolId() { return PoolId; }
        QString getToken0() { return Token0; }
        QString getToken1() { return Token1; }
        QList<QPair<QString, double>> getBalances() { return Balances; }
        bool getValid() { return Valid;};
    private:
        qint64 Height = 0;
        QString PoolId;
        QString Token0;
        QString Token1;
        QList<QPair<QString, double>> Balances;
        bool Valid = false;
    };

    class AddLiquidity {
    public:
        AddLiquidity(QString data);
        ~AddLiquidity();
        qint64 getHeight() { return Height; }
        QString getPoolId() { return PoolId; }
        QString getToken0() { return Token0; }
        QString getToken1() { return Token1; }
        QList<QPair<QString, double>> getBalances() { return Balances; }
        bool getValid() { return Valid;};
    private:
        qint64 Height = 0;
        QString PoolId;
        QString Token0;
        QString Token1;
        QList<QPair<QString, double>> Balances;
        bool Valid = false;
    };

    class RemoveLiquidity {
    public:
        RemoveLiquidity(QString data);
        ~RemoveLiquidity();
        QList<QPair<QString, double>> getBalances() { return Balances; }
        qint64 getHeight() { return Height; }
        bool getUnreceived() { return Unreceived; }
        bool getValid() { return Valid; }
    private:
        QList<QPair<QString, double>> Balances;
        qint64 Height = 0;
        bool Unreceived = false;
        bool Valid = false;
    };

    class GetBrokerAccounts {
    public:
        typedef struct {
            QString name;
            QString type;
            double shareRatio;
            int seats;
            QString pftid;
            QString owner;
        } profitsEntry_t;

        typedef struct {
            QString name;
            QString voting;
            QString owner;
            QString stkid;
            int days;
            QString start;
            double amount;
        } stakingsEntry_t;
        GetBrokerAccounts(QString data);
        ~GetBrokerAccounts();
        QList<profitsEntry_t> getProfits() { return Profits; }
        QList<stakingsEntry_t> getStakings() { return Stakings; }
        bool getValid() { return Valid; }
    private:
        QList<profitsEntry_t> Profits;
        QList<stakingsEntry_t> Stakings;
        bool Valid = false;
    };

    class AddStaking {
    public:
        AddStaking(QString data);
        ~AddStaking();
        QString getMessage() { return Message; }
        bool getValid() { return Valid;};
    private:
        QString Message;
        bool Valid = false;
    };

    class UnStaking {
    public:
        UnStaking(QString data);
        ~UnStaking();
        QString getMessage() { return Message; }
        bool getValid() { return Valid;};
    private:
        QString Message;
        bool Valid = false;
    };

    class CreateStakingAccount {
    public:
        CreateStakingAccount(QString data);
        ~CreateStakingAccount();
        QString getName() { return Voting; };
        QString getVoting() { return Voting; };
        QString getOwner() { return Owner; };
        QString getStkid() { return Stkid; };
        int getDays() { return Days; };
        QString getStart() { return Start; };
        double getAmount() { return Amount; };
        QString getMessage() { return Message; }
        bool getCompound() { return Compound; }
        bool getValid() { return Valid;};
    private:
        QString Name;
        QString Voting;
        QString Owner;
        QString Stkid;
        int Days;
        QString Start;
        double Amount;
        QString Message;
        bool Compound;
        bool Valid = false;
    };
};



#endif // RPCCLASS_H
