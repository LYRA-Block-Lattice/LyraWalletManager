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
};

#endif // WEBCLASS_H
