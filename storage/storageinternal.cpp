#include "storageinternal.h"

#include <QFileDialog>
#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"
#include "crypto/aes.h"
#include "translate/translate.h"

void StorageInternal::walletBackUp() {
    QDir dir = QDir(QString(COMPOSE_WALLET_PARH) + QDir::separator() + "backup" + QDir::separator());
    if(!dir.exists()) {
        dir.mkdir(QString(COMPOSE_WALLET_PARH) + QDir::separator() + "backup" + QDir::separator());
    }
    QString dateTime = QDateTime::currentDateTimeUtc().toString("yyyy_MM_dd_hh_mm_ss");
    QFile::rename(COMPOSE_WALLET_NAME,
                  QString(COMPOSE_WALLET_PARH) + QDir::separator() + "backup" + QDir::separator() +
                  Global::Wallet::Name::get() + "_" + dateTime + DEFAULT_WALLET_EXTENSION);
}

Global::Errors::Errors_e StorageInternal::walletCheck(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return Global::Errors::INVALID_NAME;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly)) {
        return Global::Errors::FILE_NOT_EXISTS;
    }
    QByteArray dec = aes::aesDecrypt(f.readAll(), password);
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(dec);
    f.close();
    if(!jsonWalletFile.isObject())
        return Global::Errors::INVALID_PASSWORD;
    return Global::Errors::OK;
}

Global::Errors::Errors_e StorageInternal::walletCreate(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return Global::Errors::INVALID_NAME;
    }
    StorageCommon::makeWalletFolder();
    QString fName = COMPOSE_WALLET_NAME;
    if(QFile::exists(fName)) {
        return Global::Errors::ALREADY_EXISTS;
    }
    QByteArray enc = aes::aesEncrypt("{}", password);
    QFile f(fName);
    if(!f.open(QIODevice::WriteOnly)) {
        return Global::Errors::CANNOT_SAVE;
    }
    f.write(enc);
    f.close();
    Global::Account::clearAccountList();
    Global::Wallet::Password::set(password);
    return Global::Errors::OK;
}

Global::Errors::Errors_e StorageInternal::walletDelete(QString password) {
    if( !Global::Wallet::Password::check(password) ) {
        return Global::Errors::INVALID_PASSWORD;
    }
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return Global::Errors::INVALID_NAME;
    }
    Global::Errors::Errors_e result = walletCheck(password);
    if(result != Global::Errors::OK)
        return result;
    walletBackUp();
    Global::Account::clearAccountList();
    Global::Wallet::Password::destroy();
    return Global::Errors::OK;
}

Global::Errors::Errors_e StorageInternal::walletRead(QString password) {
    if(!Global::Check::checkWalletFileNameValid(Global::Wallet::Name::get())) {
        return Global::Errors::INVALID_NAME;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly)) {
        return Global::Errors::FILE_NOT_EXISTS;
    }
    QByteArray dec = aes::aesDecrypt(f.readAll(), password);
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(dec);
    f.close();
    if(!jsonWalletFile.isObject())
        return Global::Errors::INVALID_PASSWORD;
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
    return Global::Errors::OK;
}

Global::Errors::Errors_e StorageInternal::accountCreate(QString accountName, QString password, QString pK) {
    if(!Global::Check::checkFileNameWithSpacesValid(accountName)) {
        return Global::Errors::INVALID_NAME;
    }
    if( !Global::Wallet::Password::check(password) ) {
        return Global::Errors::INVALID_PASSWORD;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly)) {
        return Global::Errors::FILE_NOT_EXISTS;
    }
    QByteArray dec = aes::aesDecrypt(f.readAll(), Global::Wallet::Password::get());
    QJsonDocument jsonWalletFileRead = QJsonDocument::fromJson(dec);
    f.close();
    if(!jsonWalletFileRead.isObject())
        return Global::Errors::INVALID_PASSWORD;
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
            return Global::Errors::INVALID_PRIVATE_KEY;
    } else {
        accountObject.insert("pk", signatures::generateAccount().privateKey);
    }
    accountsObject.insert(accountName, accountObject);
    jsonObject.insert("accounts", accountsObject);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    walletBackUp();
    if(!f.open(QIODevice::WriteOnly)) {
        return Global::Errors::FILE_NOT_EXISTS;
    }
    QByteArray enc = aes::aesEncrypt(jsonWalletFileWrite.toJson(QJsonDocument::Indented), Global::Wallet::Password::get());
    f.write(enc);
    f.close();
    Global::Error::show(nullptr, walletRead(Global::Wallet::Password::get()));
    return Global::Errors::OK;
}

Global::Errors::Errors_e StorageInternal::accountDelete(QString accountName, QString password) {
    if(!Global::Check::checkFileNameWithSpacesValid(accountName)) {
        return Global::Errors::INVALID_NAME;
    }
    if( !Global::Wallet::Password::check(password) ) {
        return Global::Errors::INVALID_PASSWORD;
    }
    QString fName = COMPOSE_WALLET_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly)) {
        return Global::Errors::FILE_NOT_EXISTS;
    }
    QByteArray dec = aes::aesDecrypt(f.readAll(), Global::Wallet::Password::get());
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(dec);
    f.close();
    if(!jsonWalletFile.isObject())
        return Global::Errors::INVALID_PASSWORD;
    Global::Account::clearAccountList();
    QJsonObject jsonObject = jsonWalletFile.object();
    QJsonObject accountsObject;
    if(jsonObject.contains("accounts")) {
        accountsObject = jsonObject["accounts"].toObject();
        if(accountsObject.contains(accountName))
            accountsObject.remove(accountName);
        else
            return Global::Errors::ACCOUNT_NOT_FOUND;
    }
    jsonObject.remove("accounts");
    jsonObject.insert("accounts", accountsObject);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    walletBackUp();
    if(!f.open(QIODevice::WriteOnly)) {
        return Global::Errors::FILE_NOT_EXISTS;
    }
    QByteArray enc = aes::aesEncrypt(jsonWalletFileWrite.toJson(QJsonDocument::Indented), Global::Wallet::Password::get());
    f.write(enc);
    f.close();
    Global::Error::show(nullptr, walletRead(Global::Wallet::Password::get()));
    return Global::Errors::OK;
}

void StorageInternal::walletExport() {
    QString fileName = QFileDialog::getSaveFileName(nullptr, Tr("Save File"),
                                                   QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                   Tr("Wallet") + " (*.lyr)"
                                                    );
    if(!QFile::copy(COMPOSE_WALLET_NAME, fileName)) {
        Global::Error::show(nullptr, Global::Errors::CANNOT_SAVE);
    }
}

bool StorageInternal::walletImport(QString name) {
    if(!Global::Check::checkFileNameWithSpacesValid(name)) {
        Global::Error::show(nullptr, Global::Errors::INVALID_NAME);
        return false;
    }
    QString fileName = QFileDialog::getOpenFileName(nullptr, Tr("Open File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    Tr("Wallet") + " (*.lyr)"
                                                    );
    if(!fileName.length()) {
        return false;
    }
    if(!QFile::copy(fileName, COMPOSE_WALLET_PARH + QDir::separator() + name + DEFAULT_WALLET_EXTENSION)) {
        Global::Error::show(nullptr, Global::Errors::CANNOT_SAVE);
        return false;
    }
    return true;
}
