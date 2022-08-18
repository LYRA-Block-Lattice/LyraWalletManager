#include "rpcclass.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <global.h>

/******************************************************************************/
RpcClass::History::History(QString data) {
    Account.Valid = false;
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

RpcClass::History::~History() {}
/******************************************************************************/
RpcClass::Balance::Balance(QString data) {
    Valid = false;
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject resultObject = jsonObject["result"].toObject();
        QJsonObject balanceObject = resultObject["balance"].toObject();
        foreach (QString key, balanceObject.keys()) {
            Balances.append(QPair<QString, double>(Global::Util::tickerToSign(key), balanceObject[key].toVariant().toDouble()));
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
        Height = resultObject["height"].toVariant().toLongLong();
        Unreceived = resultObject["unreceived"].toBool();
        Valid = true;
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::Balance::~Balance() {}
/******************************************************************************/
RpcClass::Pool::Pool(QString data) {
    Valid = false;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        if(!jsonResponse.isObject())
            return;
        QJsonObject jsonObject = jsonResponse.object();
        if(!jsonObject["result"].isNull()) {
            QJsonObject resultJsonObject = jsonObject["result"].toObject();
            if(resultJsonObject["balance"].isNull())
                return;
            Height = resultJsonObject["height"].toVariant().toLongLong();
            PoolId = resultJsonObject["poolId"].toString();
            Token0 = resultJsonObject["token0"].toString();
            Token1 = resultJsonObject["token1"].toString();
            QJsonObject balanceJsonObject = resultJsonObject["balance"].toObject();
            BalanceToken0 = balanceJsonObject[Token0].toDouble();
            BalanceToken1 = balanceJsonObject[Token1].toDouble();
            Valid = true;
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::Pool::~Pool() {}
/******************************************************************************/
RpcClass::PoolCalculate::PoolCalculate(QString data) {
    Valid = false;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        if(!jsonResponse.isObject())
            return;
        QJsonObject jsonObject = jsonResponse.object();
        if(!jsonObject["result"].isNull()) {
            QJsonObject resultJsonObject = jsonObject["result"].toObject();
            Height = resultJsonObject["Height"].toVariant().toLongLong();
            ProviderFee = resultJsonObject["ProviderFee"].toDouble();
            ProtocolFee = resultJsonObject["ProtocolFee"].toDouble();
            SwapInToken = resultJsonObject["SwapInToken"].toString();
            SwapInAmount = resultJsonObject["SwapInAmount"].toDouble();
            SwapOutToken = resultJsonObject["SwapOutToken"].toString();
            SwapOutAmount = resultJsonObject["SwapOutAmount"].toDouble();
            Price = resultJsonObject["Price"].toDouble();
            PriceImpact = resultJsonObject["PriceImpact"].toDouble();
            MinimumReceived = resultJsonObject["MinimumReceived"].toDouble();
            PayToProvider = resultJsonObject["PayToProvider"].toDouble();
            PayToAuthorizer = resultJsonObject["PayToAuthorizer"].toDouble();
            Valid = true;
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}
RpcClass::PoolCalculate::~PoolCalculate() {}
/******************************************************************************/
RpcClass::CreatePool::CreatePool(QString data) {
    Valid = false;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        if(!jsonResponse.isObject())
            return;
        QJsonObject jsonObject = jsonResponse.object();
        if(!jsonObject["result"].isNull()) {
            QJsonObject resultJsonObject = jsonObject["result"].toObject();
            if(resultJsonObject["balance"].isNull())
                return;
            Height = resultJsonObject["height"].toVariant().toLongLong();
            PoolId = resultJsonObject["poolId"].toString();
            Token0 = resultJsonObject["token0"].toString();
            Token1 = resultJsonObject["token1"].toString();
            QJsonObject balanceJsonObject = resultJsonObject["balance"].toObject();
            foreach (QString key, balanceJsonObject.keys()) {
                Balances.append(QPair<QString, double>(Global::Util::tickerToSign(key), balanceJsonObject[key].toVariant().toDouble()));
            }
            Valid = true;
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::CreatePool::~CreatePool() {}
/******************************************************************************/
RpcClass::AddLiquidity::AddLiquidity(QString data) {
    Valid = false;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        if(!jsonResponse.isObject())
            return;
        QJsonObject jsonObject = jsonResponse.object();
        if(!jsonObject["result"].isNull()) {
            QJsonObject resultJsonObject = jsonObject["result"].toObject();
            if(resultJsonObject["balance"].isNull())
                return;
            Height = resultJsonObject["height"].toVariant().toLongLong();
            PoolId = resultJsonObject["poolId"].toString();
            Token0 = resultJsonObject["token0"].toString();
            Token1 = resultJsonObject["token1"].toString();
            QJsonObject balanceJsonObject = resultJsonObject["balance"].toObject();
            foreach (QString key, balanceJsonObject.keys()) {
                Balances.append(QPair<QString, double>(Global::Util::tickerToSign(key), balanceJsonObject[key].toVariant().toDouble()));
            }
            Valid = true;
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::AddLiquidity::~AddLiquidity() {}
/******************************************************************************/
RpcClass::RemoveLiquidity::RemoveLiquidity(QString data) {
    Valid = false;
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject resultObject = jsonObject["result"].toObject();
        QJsonObject balanceObject = resultObject["balance"].toObject();
        foreach (QString key, balanceObject.keys()) {
            Balances.append(QPair<QString, double>(Global::Util::tickerToSign(key), balanceObject[key].toVariant().toDouble()));
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
        Height = resultObject["height"].toVariant().toLongLong();
        Unreceived = resultObject["unreceived"].toBool();
        Valid = true;
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::RemoveLiquidity::~RemoveLiquidity() {}
/******************************************************************************/
RpcClass::GetBrokerAccounts::GetBrokerAccounts(QString data) {
    Valid = false;
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QJsonObject resultObject = jsonObject["result"].toObject();
        QJsonArray profitsArray = resultObject["profits"].toArray();
        for(int i = 0; i < profitsArray.count(); i++) {
            QJsonObject profitObject = profitsArray[i].toObject();
            profitsEntry_t profitEntry;
            profitEntry.name = profitObject["name"].toString();
            profitEntry.type = profitObject["type"].toString();
            profitEntry.shareRatio = profitObject["shareratio"].toDouble();
            profitEntry.seats = profitObject["seats"].toInt();
            profitEntry.pftid = profitObject["pftid"].toString();
            profitEntry.owner = profitObject["owner"].toString();
            Profits.append(profitEntry);
        }
        QJsonArray stakingsArray = resultObject["stakings"].toArray();
        for(int i = 0; i < stakingsArray.count(); i++) {
            QJsonObject stakingObject = stakingsArray[i].toObject();
            stakingsEntry_t stakingEntry;
            stakingEntry.name = stakingObject["name"].toString();
            stakingEntry.voting = stakingObject["voting"].toString();
            stakingEntry.owner = stakingObject["owner"].toString();
            stakingEntry.stkid = stakingObject["stkid"].toString();
            stakingEntry.days = stakingObject["days"].toInt();
            stakingEntry.start = stakingObject["start"].toString();
            stakingEntry.amount = stakingObject["amount"].toDouble();
            Stakings.append(stakingEntry);
        }
        Valid = true;
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::GetBrokerAccounts::~GetBrokerAccounts() {}
/******************************************************************************/
RpcClass::AddStaking::AddStaking(QString data) {
    Valid = false;
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        if(jsonObject["result"].isObject()) {
            QJsonObject resultQjsonObject = jsonObject["result"].toObject();
            Valid = resultQjsonObject["success"].toBool();
            Message = resultQjsonObject["message"].toString();
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::AddStaking::~AddStaking() {}
/******************************************************************************/
RpcClass::UnStaking::UnStaking(QString data) {
    Valid = false;
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        if(jsonObject["result"].isObject()) {
            QJsonObject resultQjsonObject = jsonObject["result"].toObject();
            Valid = resultQjsonObject["success"].toBool();
            Message = resultQjsonObject["message"].toString();
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::UnStaking::~UnStaking() {}
/******************************************************************************/
RpcClass::CreateStakingAccount::CreateStakingAccount(QString data) {
    Valid = false;
    if (data.count() == 0)
        return;
    try {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        if(jsonObject["result"].isObject()) {
            QJsonObject resultQjsonObject = jsonObject["result"].toObject();
            Name = resultQjsonObject["name"].toString();
            Voting = resultQjsonObject["voting"].toString();
            Owner = resultQjsonObject["owner"].toString();
            Stkid = resultQjsonObject["stkid"].toString();
            Days = resultQjsonObject["days"].toInt();
            Start = resultQjsonObject["start"].toString();
            Amount = resultQjsonObject["amount"].toDouble();
            Compound = resultQjsonObject["compound"].toBool();
            Valid = true;
        }
    } catch (const std::exception& ex) {
        qDebug() << ex.what();
    }
}

RpcClass::CreateStakingAccount::~CreateStakingAccount() {}
/******************************************************************************/
