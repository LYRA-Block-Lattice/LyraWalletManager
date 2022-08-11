#include "webclass.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"

WebClass::CoinGecko::CoinGecko(QString data) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        QStringList keyList = jsonObject.keys();
        foreach(QString key,  keyList) {
            QJsonObject token = jsonObject[key].toObject();
            QStringList keyList = token.keys();
            foreach(QString key1,  keyList) {
                QPair<QString, double> tickerPrice;
                tickerPrice.first = Global::Util::tokenNameToTicker(key);
                tickerPrice.second = token[key1].toDouble();
                Global::TickerPrice::set(tickerPrice);
                qDebug() << Global::Util::tokenNameToTicker(key) << token[key1].toDouble();
            }
        }
    }
}
