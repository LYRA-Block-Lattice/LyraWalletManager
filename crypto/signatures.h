#ifndef SIGNATURES_H
#define SIGNATURES_H

#include <QString>

class signatures
{
public:
    typedef struct {
        QString privateKey;
        QString accountId;
    }generatedAccount;

    static QString getAccountIdFromPrivateKey(QString privKey);
    static QString getAccountIdFromPrivateKey(QByteArray privKey);
    static generatedAccount generateAccount(QByteArray key);
    static generatedAccount generateAccount();
    static bool validateAccountId(QString id);
    static bool validatePublicKey(QString pubKey);
    static bool validatePrivateKey(QString privKey);
    static bool verifyAccountSignature(QString message, QString accountId, QString signature);
    static bool verifySignature(QString message, QString accountId, QString signature);
    static QString getSignature(QString privateKey, QString message);
    static QByteArray getCheckSum(QByteArray data);
    static QByteArray verifyAndRemoveCheckSum(QByteArray data);
    static QByteArray addCheckSum(QByteArray data);
    static QString encodeWithCheckSum(QByteArray data);
    static QString encodePrivateKey(QByteArray privKey);
    static QString encodePublicKey(QByteArray pubKey);
    static QString encodeAccountId(QByteArray pubKey);
    static QByteArray decodeWithCheckSum(QString data);
    static QByteArray decodePrivateKey(QString privKey);
    static QByteArray decodePublicKey(QString pubKey);
    static QByteArray decodeAccountId(QString accountId);
};

#endif // SIGNATURES_H
