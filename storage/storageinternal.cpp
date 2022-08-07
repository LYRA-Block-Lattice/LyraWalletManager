#include "storageinternal.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"

#define COMPOSE_WALLET_PARH     QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
#define COMPOSE_WALLET_NAME     COMPOSE_WALLET_PARH + QDir::separator() + Global::Wallet::Name::get() + ".lyr"

void StorageInternal::makeWalletFolder() {
    QDir d(COMPOSE_WALLET_PARH);
    if(!d.exists(COMPOSE_WALLET_PARH))
        d.mkdir(COMPOSE_WALLET_PARH);
}

StorageInternal::storageError_e StorageInternal::walletCreate(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return INVALID_NAME;
    }
    makeWalletFolder();
    QString fName = COMPOSE_WALLET_NAME;
    if(QFile::exists(fName)) {
        return ALREADY_EXISTS;
    }
    QFile f(fName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return CANNOT_SAVE;
    }
    f.write("{}");
    f.close();
    Global::Account::clearAccountList();
    Global::Wallet::Password::set(password);
    return StorageInternal::OK;
}

StorageInternal::storageError_e StorageInternal::walletDelete(QString password) {
    if( !Global::Wallet::Password::check(password) ) {
        return INVALID_PASSWORD;
    }
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return INVALID_NAME;
    }
    QString fName = COMPOSE_WALLET_NAME;
    if(!QFile::exists(fName)) {
        return FILE_NOT_EXISTS;
    }
    QFile f(fName);
    if(!f.remove())
        return FILE_PROTECTED;
    Global::Account::clearAccountList();
    Global::Wallet::Password::destroy();
    return OK;
}

StorageInternal::storageError_e StorageInternal::walletRead(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return INVALID_NAME;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return FILE_NOT_EXISTS;
    }
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFile.isObject())
        return INVALID_FILE;
    Global::Account::clearAccountList();
    QJsonObject jsonObject = jsonWalletFile.object();
    QJsonObject accountsObject;
    if(jsonObject.contains("accounts")) {
        accountsObject = jsonObject["accounts"].toObject();
        foreach(QString accountKey, accountsObject.keys()) {
            Global::Account::addAccount(accountKey, accountsObject[accountKey].toObject()["pk"].toString(), signatures::getAccountIdFromPrivateKey(accountsObject[accountKey].toObject()["pk"].toString()));
        }
    }
    Global::Wallet::Password::set(password);
    return OK;
}

StorageInternal::storageError_e StorageInternal::accountCreate(QString accountName, QString password, QString pK) {
    if(!Global::Check::checkFileNameWithSpacesValid(accountName)) {
        return INVALID_NAME;
    }
    if( !Global::Wallet::Password::check(password) ) {
        return INVALID_PASSWORD;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return FILE_NOT_EXISTS;
    }
    QJsonDocument jsonWalletFileRead = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFileRead.isObject())
        return INVALID_FILE;
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
            return INVALID_PRIVATE_KEY;
    } else {
        accountObject.insert("pk", signatures::generateAccount().privateKey);
    }
    accountsObject.insert(accountName, accountObject);
    jsonObject.insert("accounts", accountsObject);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return FILE_NOT_EXISTS;
    }
    f.write(jsonWalletFileWrite.toJson(QJsonDocument::Indented));
    f.close();
    walletRead(password);
    return OK;
}

StorageInternal::storageError_e StorageInternal::accountDelete(QString accountName, QString password) {
    if(!Global::Check::checkFileNameWithSpacesValid(accountName)) {
        return INVALID_NAME;
    }
    if( !Global::Wallet::Password::check(password) ) {
        return INVALID_PASSWORD;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return FILE_NOT_EXISTS;
    }
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFile.isObject())
        return INVALID_FILE;
    Global::Account::clearAccountList();
    QJsonObject jsonObject = jsonWalletFile.object();
    QJsonObject accountsObject;
    if(jsonObject.contains("accounts")) {
        accountsObject = jsonObject["accounts"].toObject();
        if(accountsObject.contains(accountName))
            accountsObject.remove(accountName);
        else
            return ACCOUNT_NOT_FOUND;
    }
    jsonObject.remove("accounts");
    jsonObject.insert("accounts", accountsObject);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return FILE_NOT_EXISTS;
    }
    f.write(jsonWalletFileWrite.toJson(QJsonDocument::Indented));
    f.close();
    walletRead(password);
    return OK;
}
