#include "storageinternal.h"

#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"

StorageCommon::storageError_e StorageInternal::walletCreate(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return StorageCommon::INVALID_NAME;
    }
    StorageCommon::makeWalletFolder();
    QString fName = COMPOSE_WALLET_NAME;
    if(QFile::exists(fName)) {
        return StorageCommon::ALREADY_EXISTS;
    }
    QFile f(fName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return StorageCommon::CANNOT_SAVE;
    }
    f.write("{}");
    f.close();
    Global::Account::clearAccountList();
    Global::Wallet::Password::set(password);
    return StorageCommon::OK;
}

StorageCommon::storageError_e StorageInternal::walletDelete(QString password) {
    if( !Global::Wallet::Password::check(password) ) {
        return StorageCommon::INVALID_PASSWORD;
    }
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return StorageCommon::INVALID_NAME;
    }
    QString fName = COMPOSE_WALLET_NAME;
    if(!QFile::exists(fName)) {
        return StorageCommon::FILE_NOT_EXISTS;
    }
    QFile f(fName);
    if(!f.remove())
        return StorageCommon::FILE_PROTECTED;
    Global::Account::clearAccountList();
    Global::Wallet::Password::destroy();
    return StorageCommon::OK;
}

StorageCommon::storageError_e StorageInternal::walletRead(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return StorageCommon::INVALID_NAME;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return StorageCommon::FILE_NOT_EXISTS;
    }
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFile.isObject())
        return StorageCommon::INVALID_FILE;
    Global::Account::clearAccountList();
    QJsonObject jsonObject = jsonWalletFile.object();
    QJsonObject accountsObject;
    if(jsonObject.contains("accounts")) {
        accountsObject = jsonObject["accounts"].toObject();
        foreach(QString accountKey, accountsObject.keys()) {
            Global::Account::addAccount(accountKey, accountsObject[accountKey].toObject()["pk"].toString(),
                    signatures::getAccountIdFromPrivateKey(accountsObject[accountKey].toObject()["pk"].toString()));
        }
    }
    Global::Wallet::Password::set(password);
    return StorageCommon::OK;
}

StorageCommon::storageError_e StorageInternal::accountCreate(QString accountName, QString password, QString pK) {
    if(!Global::Check::checkFileNameWithSpacesValid(accountName)) {
        return StorageCommon::INVALID_NAME;
    }
    if( !Global::Wallet::Password::check(password) ) {
        return StorageCommon::INVALID_PASSWORD;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return StorageCommon::FILE_NOT_EXISTS;
    }
    QJsonDocument jsonWalletFileRead = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFileRead.isObject())
        return StorageCommon::INVALID_FILE;
    Global::Account::clearAccountList();
    QJsonObject jsonObject = jsonWalletFileRead.object();
    QJsonObject accountsObject;
    QJsonObject accountObject;
    if(jsonObject.contains("accounts")) {
        accountsObject = jsonObject["accounts"].toObject();
        jsonObject.remove("accounts");
    }
    if(pK != nullptr) {
        if(signatures::validatePrivateKey(pK))
            accountObject.insert("pk", pK);
        else
            return StorageCommon::INVALID_PRIVATE_KEY;
    } else {
        accountObject.insert("pk", signatures::generateAccount().privateKey);
    }
    accountsObject.insert(accountName, accountObject);
    jsonObject.insert("accounts", accountsObject);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return StorageCommon::FILE_NOT_EXISTS;
    }
    f.write(jsonWalletFileWrite.toJson(QJsonDocument::Indented));
    f.close();
    walletRead(password);
    return StorageCommon::OK;
}

StorageCommon::storageError_e StorageInternal::accountDelete(QString accountName, QString password) {
    if(!Global::Check::checkFileNameWithSpacesValid(accountName)) {
        return StorageCommon::INVALID_NAME;
    }
    if( !Global::Wallet::Password::check(password) ) {
        return StorageCommon::INVALID_PASSWORD;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return StorageCommon::FILE_NOT_EXISTS;
    }
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFile.isObject())
        return StorageCommon::INVALID_FILE;
    Global::Account::clearAccountList();
    QJsonObject jsonObject = jsonWalletFile.object();
    QJsonObject accountsObject;
    if(jsonObject.contains("accounts")) {
        accountsObject = jsonObject["accounts"].toObject();
        if(accountsObject.contains(accountName))
            accountsObject.remove(accountName);
        else
            return StorageCommon::ACCOUNT_NOT_FOUND;
    }
    jsonObject.remove("accounts");
    jsonObject.insert("accounts", accountsObject);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return StorageCommon::FILE_NOT_EXISTS;
    }
    f.write(jsonWalletFileWrite.toJson(QJsonDocument::Indented));
    f.close();
    walletRead(password);
    return StorageCommon::OK;
}
