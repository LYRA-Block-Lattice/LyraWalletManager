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
                //qDebug() << Global::Util::tokenNameToTicker(key) << token[key1].toDouble();
            }
        }
        valid = true;
    }
}

WebClass::YourShareInPool::YourShareInPool(QString data) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        PoolFactoryAccountId = jsonObject["poolFactoryAccountId"].toString();
        PoolAccountId = jsonObject["poolAccountId"].toString();
        Token0 = jsonObject["token0"].toString();
        Token1 = jsonObject["token1"].toString();
        QString blockDataString = jsonObject["blockData"].toString();
        QJsonDocument blockDataJson = QJsonDocument::fromJson(blockDataString.toUtf8());
        QJsonObject blockDataObj = blockDataJson.object();
        RelatedTx = blockDataObj["RelatedTx"].toString();
        if (!blockDataObj["Shares"].isNull()) {
            QJsonObject shares = blockDataObj["Shares"].toObject();
            foreach (QString key, shares.keys()) {
                Shares.append(QPair<QString, double>(key, ((double) shares[key].toVariant().toLongLong()) / 1000000000000.0));
            }
        }

        DestinationAccountId = blockDataObj["DestinationAccountId"].toString();
        AccountID = blockDataObj["AccountID"].toString();
        QJsonObject balances = blockDataObj["Balances"].toObject();
        foreach (QString key, balances.keys()) {
            Balances.append(QPair<QString, double>(key, ((double) balances[key].toVariant().toLongLong()) / 1000000000000.0));
        }
        Fee = blockDataObj["Fee"].toDouble();
        FeeCode = blockDataObj["FeeCode"].toString();
        FeeType = blockDataObj["FeeType"].toInt();
        NonFungibleToken = blockDataObj["NonFungibleToken"].toString();
        VoteFor = blockDataObj["VoteFor"].toString();
        Height = blockDataObj["Height"].toVariant().toLongLong();
        TimeStamp = blockDataObj["TimeStamp"].toString();
        /*java.sql.Timestamp ts = java.sql.Timestamp.valueOf(
                blockDataObj.getString("TimeStamp").replace("T", " ").split("\\.")[0]);
        Time = ts.getTime();*/
        Version = blockDataObj["Version"].toInt();
        BlockType = blockDataObj["BlockType"].toInt();
        PreviousHash = blockDataObj["PreviousHash"].toString();
        ServiceHash = blockDataObj["ServiceHash"].toString();
        if (!blockDataObj["Tags"].isNull()) {
            QJsonObject tags = blockDataObj["Tags"].toObject();
            foreach (QString key, tags.keys()) {
                Tags.append(QPair<QString, QString>(key, tags[key].toString()));
            }
        }
        Hash = blockDataObj["Hash"].toString();
        Signature = blockDataObj["Signature"].toString();
        ResultBlockType = jsonObject["resultBlockType"].toInt();
        ResultCode = jsonObject["resultCode"].toInt();
        ResultMessage = jsonObject["resultMessage"].toString();

        valid = true;
    }
}
