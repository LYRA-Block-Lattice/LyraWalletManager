#include "storagesettings.h"

#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"
#include "storagecommon.h"

void StorageSettings::createDefault(bool everwrite) {
    StorageCommon::makeSettingsFolder();
    QString fName = COMPOSE_SETTINGS_NAME;
    if(!everwrite) {
        if(QFile::exists(fName)) {
            return ;
        }
    }
    QFile f(fName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    f.write("{}");
    f.close();
}

void StorageSettings::set(QString key, QString value) {
    QString fName = COMPOSE_SETTINGS_NAME;
    QFile f(fName);
    createDefault();
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QJsonDocument jsonWalletFileRead = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFileRead.isObject()) {
        createDefault(true);
    }
    QJsonObject jsonObject = jsonWalletFileRead.object();

    if(!jsonObject[key].isNull())
        jsonObject.remove(key);

    jsonObject.insert(key, value);
    QJsonDocument jsonWalletFileWrite(jsonObject);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    f.write(jsonWalletFileWrite.toJson(QJsonDocument::Indented));
    f.close();
}

QString StorageSettings::get(QString key, QString defaultValue) {
    QString fName = COMPOSE_SETTINGS_NAME;
    QFile f(fName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return defaultValue;
    }
    QJsonDocument jsonWalletFileRead = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonWalletFileRead.isObject()) {
        createDefault(true);
    }
    QJsonObject jsonObject = jsonWalletFileRead.object();
    if(jsonObject[key].isNull())
        return defaultValue;
    else
        return jsonObject[key].toString();
}

