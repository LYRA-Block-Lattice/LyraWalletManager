#include "rpcclass.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <global.h>

/******************************************************************************/
RpcClass::History::History(QString data) {
    if (data.count() == 0)
        return;
    Account.AccountName = Global::Account::getSelectedAccountName();
    Account.NetworkName = Global::Network::getNetwork();
    Account.HistoryData = data;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray arrayObject = jsonObject["result"].toArray();
        for (int i = 0; i < arrayObject.count(); i++) {
            QJsonObject obj = arrayObject[i].toObject();
            RpcClass::History::entry_t entry;
            entry.Height = obj["Height"].toVariant().toLongLong();
            entry.IsReceive = obj["IsReceive"].toBool();
            entry.TimeStamp = obj["TimeStamp"].toVariant().toLongLong();
            if(obj.contains("SendAccountId"))
                entry.SendAccountId = obj["SendAccountId"].toString();
            if(obj.contains("SendHash"))
                entry.SendHash = obj["SendHash"].toString();
            entry.RecvAccountId = obj["RecvAccountId"].toString();
            if(obj.contains("RecvHash"))
                entry.RecvHash = obj["RecvHash"].toString();

            QJsonObject o = obj["Changes"].toObject();
            foreach (QString key, o.keys()) {
                entry.Changes.append(QPair<QString, double>(Global::Util::tickerToSign(key), o[key].toVariant().toDouble()));
            }
            if(entry.Changes.size() > 1) {
                for (int i = 0; i < entry.Changes.size(); i++) {
                    QPair<QString, double> p = entry.Changes[i];
                    if(!p.first.compare("LYR")) {
                        // Make sure LYR is on last position
                        entry.Changes.removeAt(i);
                        entry.Changes.append(p);
                        break;
                    }
                }
            }
            o = obj["Balances"].toObject();
            foreach (QString key, o.keys()) {
                entry.Balances.append(QPair<QString, double>(Global::Util::tickerToSign(key), o[key].toVariant().toDouble()));
            }
            if(entry.SendAccountId.count() != 0) {
                if (entry.Balances.size() > 1) {
                    for (int i = 1; i < entry.Balances.size(); i++) {
                        QPair<QString, double> p = entry.Balances[i];
                        if (!p.first.compare("LYR")) {
                            // Make sure LYR is on position 0
                            entry.Balances.removeAt(i);
                            entry.Balances.insert(0, p);
                            break;
                        }
                    }
                }
            }
            if(entry.SendAccountId.count() == 0) { // Is a mint block, we need to display the new minted amount.
                QPair<QString, double> p = entry.Changes[0];
                entry.Changes.removeAt(0);
                entry.Changes.append(p);
            }
            Account.History.append(entry);
        }
        Account.Valid = true;
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::History::History(RpcClass::History *history) {
    Account = history->Account;
}

RpcClass::History::~History() {
}
/******************************************************************************/
RpcClass::Balance::Balance(QString data) {
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject resultObject = jsonObject["result"].toObject();
        QJsonObject o = resultObject["balance"].toObject();
        foreach (QString key, o.keys()) {
            Balances.append(QPair<QString, double>(Global::Util::tickerToSign(key), o[key].toVariant().toDouble()));
        }
        if (Balances.size() > 1) {
            for (int i = 1; i < Balances.size(); i++) {
                QPair<QString, double> p = Balances[i];
                if (!p.first.compare("LYR")) {
                    // Make sure LYR is on position 0
                    Balances.removeAt(i);
                    Balances.insert(0, p);
                    break;
                }
            }
        }
        height = resultObject["height"].toVariant().toLongLong();
        unreceived = resultObject["unreceived"].toBool();
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::Balance::~Balance() {
}
/******************************************************************************/
RpcClass::Pool::Pool(QString data) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(!jsonResponse.isObject())
        return;
    QJsonObject jsonObject = jsonResponse.object();
    if(!jsonObject["result"].isNull()) {
        QJsonObject resultJsonObject = jsonObject["result"].toObject();
        if(resultJsonObject["balance"].isNull())
            return;
        height = resultJsonObject["height"].toVariant().toLongLong();
        poolId = resultJsonObject["poolId"].toString();
        token0 = resultJsonObject["token0"].toString();
        token1 = resultJsonObject["token1"].toString();
        QJsonObject balanceJsonObject = resultJsonObject["balance"].toObject();
        balanceToken0 = balanceJsonObject[token0].toDouble();
        balanceToken1 = balanceJsonObject[token1].toDouble();
        valid = true;
    }
}

RpcClass::Pool::~Pool() {
}
/******************************************************************************/
RpcClass::PoolCalculate::PoolCalculate(QString data) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(!jsonResponse.isObject())
        return;
    QJsonObject jsonObject = jsonResponse.object();
    if(!jsonObject["result"].isNull()) {
        QJsonObject resultJsonObject = jsonObject["result"].toObject();
        height = resultJsonObject["Height"].toVariant().toLongLong();
        providerFee = resultJsonObject["ProviderFee"].toDouble();
        protocolFee = resultJsonObject["ProtocolFee"].toDouble();
        swapInToken = resultJsonObject["SwapInToken"].toString();
        swapInAmount = resultJsonObject["SwapInAmount"].toDouble();
        swapOutToken = resultJsonObject["SwapOutToken"].toString();
        swapOutAmount = resultJsonObject["SwapOutAmount"].toDouble();
        price = resultJsonObject["Price"].toDouble();
        priceImpact = resultJsonObject["PriceImpact"].toDouble();
        minimumReceived = resultJsonObject["MinimumReceived"].toDouble();
        payToProvider = resultJsonObject["PayToProvider"].toDouble();
        payToAuthorizer = resultJsonObject["PayToAuthorizer"].toDouble();
        valid = true;
    }
}

RpcClass::PoolCalculate::~PoolCalculate() {
}

