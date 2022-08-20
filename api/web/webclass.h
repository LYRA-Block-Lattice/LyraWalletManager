#ifndef WEBCLASS_H
#define WEBCLASS_H

#include <QString>
#include <QList>
#include <QPair>

namespace WebClass {
    class CoinGecko {
    private:
        bool valid = false;
    public:
        CoinGecko(QString data);
        bool getValid() { return valid; }
    };
    class YourShareInPool {
    private:
        QString PoolFactoryAccountId;
        QString PoolAccountId;
        QString Token0;
        QString Token1;

        QString RelatedTx;
        QList<QPair<QString, double>> Shares = QList<QPair<QString, double>>();

        QString DestinationAccountId;
        QString AccountID;
        QList<QPair<QString, double>> Balances = QList<QPair<QString, double>>();
        double Fee = 0.0;
        QString FeeCode;
        int FeeType = 0;
        QString NonFungibleToken;
        QString VoteFor;
        qint64 Height = 0;
        QString TimeStamp;
        qint64 Time = 0;
        int Version = 0;
        int BlockType = 0;
        QString PreviousHash;
        QString ServiceHash;
        QList<QPair<QString, QString>> Tags = QList<QPair<QString, QString>>();
        QString Hash;
        QString Signature;
        int ResultBlockType = 0;
        int ResultCode = 0;
        QString ResultMessage;

        bool valid = false;
    public:
        YourShareInPool(QString data);
        QString getPoolFactoryAccountId() { return PoolFactoryAccountId; }
        QString getPoolAccountId() { return PoolAccountId; }
        QString getToken0() { return Token0; }
        QString getToken1() { return Token1; }

        QString getRelatedTx() { return RelatedTx; }
        QList<QPair<QString, double>> getShares() { return Shares; }

        QString getDestinationAccountId() { return DestinationAccountId; }
        QString getAccountID() { return AccountID; }
        QList<QPair<QString, double>> getBalances() { return Balances; }
        double getFee() { return Fee; }
        QString getFeeCode() { return FeeCode; }
        int getFeeType() { return FeeType; }
        QString getNonFungibleToken() { return NonFungibleToken; }
        QString getVoteFor() { return VoteFor; }
        long getHeight() { return Height; }
        QString getTimeStamp() { return TimeStamp; }
        int getVersion() { return Version; }
        int getBlockType() { return BlockType; }
        QString getPreviousHash() { return PreviousHash; }
        QString getServiceHash() { return ServiceHash; }
        QList<QPair<QString, QString>> getTags() { return Tags; }
        QString getHash() { return Hash; }
        QString getSignature() { return Signature; }
        int getResultBlockType() { return ResultBlockType; }
        int getResultCode() { return ResultCode; }
        QString getResultMessage() { return ResultMessage; }

        bool getValid() { return valid; }
    };

    class AllProfitingAccounts {
    public:
        typedef struct {
            int AccountType;
            QString PType;
            double ShareRatio;
            int Seats;
            QString ProfitHash;
            QString Name;
            QString OwnerAccountId ;
            QString RelatedTx;
            QString SourceHash;
            QString AccountId;
            QList<QPair<QString, double>> Balances;
            double Fee;
            QString FeeCode;
            int FeeType;
            QString NonFungibleToken;
            QString VoteFor;
            qint64 Height ;
            QString TimeStamp;
            int Version;
            int BlockType;
            QString PreviousHash;
            QString ServiceHash;
            QList<QPair<QString, QString>> Tags;
            QString Hash;
            QString Signature;
            double TotalProfit;
        } entry_t;
    private:
        QList<entry_t> AccountList;
        bool Valid = false;
    public:

        AllProfitingAccounts(QString data);
        QList<entry_t> getAccountList() { return AccountList; }
        bool getValid() { return Valid; }
    };
    class AllStakings {
    public:
        typedef struct {
            QString StkAccount;
            QString OwnerAccount;
            QString Time;
            int Days;
            double Amount;
            bool CompoundMode;
        } entry_t;
    private:
        QList<entry_t> StakingList;
        bool Valid = false;
    public:
        AllStakings(QString data);
        QList<entry_t> getStakingList() { return StakingList; }
        bool getValid() { return Valid; }
    };
    class DexGetSuportedExtTokens {
    public:
        typedef struct {
            QString Name;
            QString CoinGeckoName;
            QString Url;
            QString Symbol;
            QString NetworkProvider;
            QString Contract;
            double MinDeposit;
            double DepositFee;
            QString ConfirmationInfo;
            double MinWithdraw;
            double WithdrawFee;
            double DailyWithdrawLimit;
        }entry_t;
    private:
        QList<entry_t> EntryList;
        bool Success = false;
        QString Message = "";
        bool Valid = false;
    public:
        DexGetSuportedExtTokens(QString data);
        QList<entry_t> getEntryList() { return EntryList;}
        bool getSuccess() { return Success;}
        QString getMessage() { return Message;}
        bool getValid() { return Valid;}
    };
    class DexGetAllWallets {
    public:
        typedef struct {
            int AccountType;
            QString IntSymbol;
            QString ExtSymbol;
            QString ExtProvider;
            QString ExtAddress;
            QString OwnerAccountId;
            QString RelatedTx;
            QString Name;
            QString DestinationAccountId;
            QString AccountID;
            QList<QPair<QString, double>> Balances;
            double Fee;
            QString FeeCode;
            int FeeType;
            QString NonFungibleToken;
            QString VoteFor;
            long Height;
            QString TimeStamp;
            int Version;
            int BlockType;
            QString PreviousHash;
            QString ServiceHash;
            QList<QPair<QString, QString>> Tags;
            QString Hash;
            QString Signature;
        }entry_t;
    private:
        QList<entry_t> EntryList;
        int ResultCode = -1;
        QString ResultMessage;
        bool Valid = false;
    public:
        DexGetAllWallets(QString data);
        QList<entry_t> getEntryList() { return EntryList;}
        int getResultCode() { return ResultCode;}
        QString getResultMessage() { return ResultMessage; }
        bool getValid() { return Valid;}
    };
};

#endif // WEBCLASS_H
