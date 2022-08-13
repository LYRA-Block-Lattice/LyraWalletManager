#include "translate.h"

#include <QList>
#include <QPair>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QList<QPair<QString, QString>> list = QList<QPair<QString, QString>>();

void setLanguage(QString language) {
    QFile f(":/language/translate/languages/" + language + ".tr");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        list.clear();
        return;
    }
    QJsonDocument jsonFile = QJsonDocument::fromJson(f.readAll());
    f.close();
    if(!jsonFile.isObject()) {
        list.clear();
        return;
    }
    QJsonObject jsonObject = jsonFile.object();
    QList<QString> keys = jsonObject.keys();
    foreach(QString key, keys) {
        list.append(QPair<QString, QString>(key, jsonObject[key].toString()));
    }
}

QString Translate::tr(QString s) {
    QPair<QString, QString> i;
    foreach(i, list) {
        if(!i.first.compare(s)) {
            return i.second;
        }
    }
    return s;
}

QString Tr(QString s) {
    QPair<QString, QString> i;
    foreach(i, list) {
        if(!i.first.compare(s)) {
            return i.second;
        }
    }
    return s;
}
