#include "webclass.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "global.h"

WebClass::CoinGecko::CoinGecko(QString data) {
    valid = false;
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
    valid = false;
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

WebClass::AllProfitingAccounts::AllProfitingAccounts(QString data) {
    Valid = false;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(jsonResponse.isArray()) {
        QJsonArray jsonArray = jsonResponse.array();
        QJsonObject accObj;
        for(int i = 0;i < jsonArray.count(); i++) {
            accObj = jsonArray[i].toObject();
            entry_t entry;
            QJsonObject gensObj = accObj["gens"].toObject();
            entry.AccountType = gensObj["accountType"].toInt();
            entry.PType = profitingAccountTypesList[gensObj["pType"].toInt()];
            entry.ShareRatio = gensObj["shareRito"].toDouble();
            entry.Seats = gensObj["seats"].toInt();
            entry.ProfitHash = gensObj["profitHash"].toString();
            entry.Name = gensObj["name"].toString();
            entry.OwnerAccountId = gensObj["ownerAccountId"].toString();
            entry.RelatedTx = gensObj["relatedTx"].toString();
            entry.SourceHash = gensObj["sourceHash"].toString();
            entry.AccountId = gensObj["accountID"].toString();
            if (!gensObj["balances"].isNull()) {
                QJsonObject balances = gensObj["balances"].toObject();
                foreach (QString key, balances.keys()) {
                    entry.Balances.append(QPair<QString, double>(key, balances[key].toDouble()));
                }
            }
            entry.Fee = gensObj["fee"].toDouble();
            entry.FeeCode = gensObj["feeCode"].toString();
            entry.FeeType = gensObj["feeType"].toInt();
            entry.NonFungibleToken = gensObj["nonFungibleToken"].toString();
            entry.VoteFor = gensObj["voteFor"].toString();;
            entry.Height = gensObj["height"].toVariant().toULongLong();
            //java.sql.Timestamp ts = java.sql.Timestamp.valueOf(
                    //gensObj.getString("timeStamp").replace("T", " ").split("\\.")[0]);
            //entry.TimeStamp = ts.getTime();
            entry.TimeStamp = gensObj["timeStamp"].toString();
            entry.Version = gensObj["version"].toInt();
            entry.BlockType = gensObj["blockType"].toInt();
            entry.PreviousHash = gensObj["previousHash"].toString();
            entry.ServiceHash = gensObj["serviceHash"].toString();
            if (!gensObj["tags"].isNull()) {
                QJsonObject tags = gensObj["tags"].toObject();
                foreach (QString key, tags.keys()) {
                    entry.Tags.append(QPair<QString, QString>(key, tags[key].toString()));
                }
            }
            entry.Hash = gensObj["hash"].toString();
            entry.Signature = gensObj["signature"].toString();
            entry.TotalProfit = accObj["totalprofit"].toDouble();
            AccountList.append(entry);
        }
        Valid = true;
    }
}

WebClass::AllStakings::AllStakings(QString data) {
    Valid = false;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(jsonResponse.isArray()) {
        QJsonArray jsonArray = jsonResponse.array();
        QJsonObject accObj;
        for(int i = 0;i < jsonArray.count(); i++) {
            accObj = jsonArray[i].toObject();
            entry_t entry;
            entry.StkAccount = accObj["stkAccount"].toString();
            entry.OwnerAccount = accObj["ownerAccount"].toString();
            //java.sql.Timestamp ts = java.sql.Timestamp.valueOf(
            //        accObj.getString("time").replace("T", " ").split("\\.")[0]);
            entry.Time = accObj["time"].toString();
            entry.Days = accObj["days"].toInt();
            entry.Amount = accObj["amount"].toDouble();
            entry.CompoundMode = accObj["compoundMode"].toBool();
            StakingList.append(entry);
        }
        Valid = true;
    }
}

WebClass::DexGetSuportedExtTokens::DexGetSuportedExtTokens(QString data)  {
    Valid = false;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray jsonArray = jsonObject["asserts"].toArray();
        for( int i = 0; i < jsonArray.count(); i++) {
            QJsonObject assertsJsonObject = jsonArray[i].toObject();
            entry_t entry;
            entry.Name = assertsJsonObject["name"].toString();
            entry.CoinGeckoName = assertsJsonObject["coinGeckoName"].toString();
            entry.Url = assertsJsonObject["url"].toString();
            entry.Symbol = assertsJsonObject["symbol"].toString();
            entry.NetworkProvider = assertsJsonObject["networkProvider"].toString();
            entry.Contract = assertsJsonObject["contract"].toString();
            entry.MinDeposit = assertsJsonObject["minDeposit"].toDouble();
            entry.DepositFee = assertsJsonObject["depositFee"].toDouble();
            entry.ConfirmationInfo = assertsJsonObject["confirmationInfo"].toString();
            entry.MinWithdraw = assertsJsonObject["minWithdraw"].toDouble();
            entry.WithdrawFee = assertsJsonObject["withdrawFee"].toDouble();
            entry.DailyWithdrawLimit = assertsJsonObject["dailyWithdrawLimit"].toDouble();
            EntryList.append(entry);
        }
        Success = jsonObject["success"].toBool();
        Message = jsonObject["message"].toString();
        Valid = true;
    }
}

WebClass::DexGetAllWallets::DexGetAllWallets(QString data) {
    Valid = false;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
    if(jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        QJsonArray jsonArray = jsonObject["blockDatas"].toArray();
        for( int i = 0; i < jsonArray.count(); i++) {
            QJsonObject assertsJsonObject = jsonArray[i].toObject();
            QString ent = jsonArray[i].toString();
            QJsonDocument tokenJsonResponse = QJsonDocument::fromJson(ent.toUtf8());
            QJsonObject tokenObj = tokenJsonResponse.object();
            entry_t entry;
            if(!tokenObj["AccountType"].isNull())
                entry.AccountType = tokenObj["AccountType"].toInt();
            entry.IntSymbol = tokenObj["IntSymbol"].toString();
            entry.ExtSymbol = tokenObj["ExtSymbol"].toString();
            entry.ExtProvider = tokenObj["ExtProvider"].toString();
            entry.ExtAddress = tokenObj["ExtAddress"].toString();
            entry.OwnerAccountId = tokenObj["OwnerAccountId"].toString();
            entry.RelatedTx = tokenObj["RelatedTx"].toString();
            entry.Name = tokenObj["Name"].toString();
            if(!tokenObj["DestinationAccountId"].isNull())
                entry.DestinationAccountId = tokenObj["DestinationAccountId"].toString();
            entry.AccountID = tokenObj["AccountID"].toString();
            if (!tokenObj["Balances"].isNull()) {
                QJsonObject balances = tokenObj["Balances"].toObject();
                foreach (QString key, balances.keys()) {
                    entry.Balances.append(QPair<QString, double>(key, balances[key].toDouble()));
                }
            }
            entry.Fee = tokenObj["Fee"].toDouble();
            entry.FeeCode = tokenObj["FeeCode"].toString();
            entry.FeeType = tokenObj["FeeType"].toInt();
            entry.NonFungibleToken = tokenObj["NonFungibleToken"].toString();
            entry.VoteFor = tokenObj["VoteFor"].toString();
            entry.Height = tokenObj["Height"].toVariant().toLongLong();
            entry.TimeStamp = tokenObj["TimeStamp"].toString();
            entry.Version = tokenObj["Version"].toInt();
            entry.BlockType = tokenObj["BlockType"].toInt();
            entry.PreviousHash = tokenObj["PreviousHash"].toString();
            entry.ServiceHash = tokenObj["ServiceHash"].toString();
            if (!tokenObj["Tags"].isNull()) {
                QJsonObject tags = tokenObj["Tags"].toObject();
                foreach (QString key, tags.keys()) {
                    entry.Tags.append(QPair<QString, QString>(key, tags[key].toString()));
                }
            }
            entry.Hash = tokenObj["Hash"].toString();
            entry.Signature = tokenObj["Signature"].toString();
            EntryList.append(entry);
        }
        ResultCode = jsonObject["resultCode"].toInt();
        ResultMessage = jsonObject["resultMessage"].toString();
        Valid = true;
    }
}
