#include <QRandomGenerator>
#include <QCryptographicHash>
#include <QDebug>

#include "signatures.h"
#include "base58Encoding.h"
#include "ecc.h"
#include "globallyra.h"

QString signatures::getAccountIdFromPrivateKey(QString privKey) {
    QByteArray prKey = signatures::decodePrivateKey(privKey);
    EccPoint pubKey;
    ecc::make_key(&pubKey, (uint8_t *)prKey.data(), (uint8_t *)prKey.data());
    QByteArray puKey;
    puKey.append((char *)&pubKey.x, 32);
    puKey.append((char *)&pubKey.y, 32);
    return signatures::encodeAccountId(puKey);
}

QString signatures::getAccountIdFromPrivateKey(QByteArray privKey) {
    EccPoint pubKey;
    ecc::make_key(&pubKey, (uint8_t *)privKey.data(), (uint8_t *)privKey.data());
    QByteArray puKey;
    puKey.append((char *)&pubKey.x, 32);
    puKey.append((char *)&pubKey.y, 32);
    return signatures::encodeAccountId(puKey);
}

signatures::generatedAccount signatures::generateAccount(QByteArray key) {
    signatures::generatedAccount acc;
    acc.accountId = getAccountIdFromPrivateKey(key);
    acc.privateKey = signatures::encodePrivateKey(key);
    return acc;
}

signatures::generatedAccount signatures::generateAccount() {
    quint32 array[8];
    QRandomGenerator::global()->fillRange(array);
    QByteArray key;
    for(quint32 tmp: array) {
        key.append((tmp >> 24) & 0xFF);
        key.append((tmp >> 16) & 0xFF);
        key.append((tmp >> 8) & 0xFF);
        key.append(tmp & 0xFF);
    }
    return generateAccount(key);
}

bool signatures::validateAccountId(QString id) {
    if(signatures::decodeAccountId(id) == nullptr || id.at(0) != ADDRESSPREFIX)
        return false;
    else
        return true;
}

bool signatures::validatePublicKey(QString pubKey) {
    return signatures::decodePublicKey(pubKey) != nullptr;
}

bool signatures::validatePrivateKey(QString privKey) {
    return signatures::decodePrivateKey(privKey) != nullptr;
}

bool signatures::verifyAccountSignature(QString message, QString accountId, QString signature) {
    return verifySignature(message, accountId, signature);
}

bool signatures::verifySignature(QString message, QString accountId, QString signature) {
    QByteArray signatureBytes = base58Encoding::decode(signature);
    QByteArray publicKeyBytes = signatures::decodeAccountId(accountId);
    EccPoint pubKey;
    EccPoint sig;
    int cnt = 0;
    for(; cnt < 32; cnt++) {
        pubKey.x[cnt] = publicKeyBytes[cnt];
        pubKey.y[cnt] = publicKeyBytes[cnt + 32];
        sig.x[cnt] = signatureBytes[cnt];
        sig.y[cnt] = signatureBytes[cnt + 32];
    }
    QCryptographicHash sha256 (QCryptographicHash::Sha256);
    sha256.reset();
    sha256.addData(message.toUtf8(), message.length());
    QByteArray hash = sha256 .result();
    return ecc::ecdsa_verify(&pubKey, (uint8_t *)hash.data(), sig.x, sig.y);
}

QString signatures::getSignature(QString privateKey, QString message) {
    QByteArray privateKeyBytes = signatures::decodePrivateKey(privateKey);
    uint8_t pKey[NUM_ECC_DIGITS];
    int cnt = 0;
    for(; cnt < 32; cnt++)
        pKey[cnt] = privateKeyBytes[cnt];
    QCryptographicHash sha256 (QCryptographicHash::Sha256);
    sha256.reset();
    sha256.addData(message.toUtf8(), message.length());
    QByteArray hash = sha256.result();
    uint8_t sig[NUM_ECC_DIGITS * 2];
    ecc::ecdsa_sign(pKey, (uint8_t *)hash.data(), sig);
    return base58Encoding::encode(QByteArray((char *)sig, 64));
}

QByteArray signatures::getCheckSum(QByteArray data) {
    QCryptographicHash sha256 (QCryptographicHash::Sha256);
    sha256.reset();
    sha256.addData(data);
    QByteArray hash1 = sha256.result();
    sha256.reset();
    sha256 .addData(hash1);
    QByteArray hash2 = sha256.result();
    QByteArray ret;
    for(int cnt = 0; cnt < LYRA_CHECKSUM_SIZE_IN_BYTES; cnt ++) {
        ret.append(hash2.at(cnt));
    }
    return ret;
}

QByteArray signatures::verifyAndRemoveCheckSum(QByteArray data) {
    if(data.count() < LYRA_CHECKSUM_SIZE_IN_BYTES)
        return QByteArray();
    QByteArray tmpData;
    for (int cnt = 0; cnt < data.count() - LYRA_CHECKSUM_SIZE_IN_BYTES; cnt++) {
        tmpData.append(data.at(cnt));
    }
    QByteArray checkSumCalc = getCheckSum(tmpData);
    QByteArray checkSumData;
    for (int cnt = data.count() - LYRA_CHECKSUM_SIZE_IN_BYTES; cnt < data.count(); cnt++) {
        checkSumData.append(data.at(cnt));
    }
    if(!checkSumCalc.compare(checkSumData))
        return tmpData;
    else
        return nullptr;
}

QByteArray signatures::addCheckSum(QByteArray data) {
    QByteArray tmpData;
    for (int cnt = 0; cnt < data.count(); cnt++) {
        tmpData.append(data.at(cnt));
    }
    QByteArray checkSum = getCheckSum(tmpData);
    return data + checkSum;
}

QString signatures::encodeWithCheckSum(QByteArray data) {
    QByteArray tmpData;
    for (int cnt = 0; cnt < data.count(); cnt++) {
        tmpData.append(data.at(cnt));
    }
    QByteArray checkSum = getCheckSum(tmpData);
    return base58Encoding::encode(data + checkSum);
}

QString signatures::encodePrivateKey(QByteArray privKey) {
    return encodeWithCheckSum(privKey);
}

QString signatures::encodePublicKey(QByteArray pubKey) {
    return encodeWithCheckSum(pubKey);
}

QString signatures::encodeAccountId(QByteArray pubKey) {
    return ADDRESSPREFIX + encodeWithCheckSum(pubKey);
}

QByteArray signatures::decodeWithCheckSum(QString data) {
    QByteArray dat = base58Encoding::decode(data);
    return verifyAndRemoveCheckSum(dat);
}

QByteArray signatures::decodePrivateKey(QString privKey) {
    return decodeWithCheckSum(privKey);
}

QByteArray signatures::decodePublicKey(QString pubKey) {
    return decodeWithCheckSum(pubKey);
}

QByteArray signatures::decodeAccountId(QString accountId) {
    return decodeWithCheckSum(accountId.mid(1)/*QStringRef(&accountId, 1, accountId.length() - 1).toString()*/);
}



