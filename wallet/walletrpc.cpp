#include "walletrpc.h"
#include "global.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDateTime>
#include "wallet/history.h"

QString WalletRpc::compose(int id, QString method, QList<QString> arg) {
    QJsonObject jsonObject;
    jsonObject.insert("jsonrpc", "2.0");
    jsonObject.insert("id", id);
    jsonObject.insert("method", method);
    QJsonArray arrayObject;
    QString s;
    foreach(s, arg) {
        arrayObject.append(s);
    }
    jsonObject.insert("params", arrayObject);
    QJsonDocument jsonDocument(jsonObject);
    return jsonDocument.toJson(QJsonDocument::Compact);
}

QString WalletRpc::composeSign(int id, QString sig) {
    QJsonObject jsonObject;
    jsonObject.insert("jsonrpc", "2.0");
    jsonObject.insert("id", id);
    QJsonArray arrayObject;
    arrayObject.append("p1393");
    arrayObject.append(sig);
    jsonObject.insert("result", arrayObject);
    QJsonDocument jsonDocument(jsonObject);
    return jsonDocument.toJson(QJsonDocument::Compact);
}

void WalletRpc::History::doWork() {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError();
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::History::operate, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, &WalletRpc::History::on_HistoryRetriveDone);
        connect(this, &WalletRpc::History::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit operate(Global::Network::getNodeAddress().second, WalletRpc::compose(1, "History", QList<QString>({Global::Account::getAccountPublicId(), "0", QString::number(QDateTime::currentMSecsSinceEpoch()), "0"})));
}

void WalletRpc::History::on_HistoryRetriveDone(const QString &s) {
    emit socketDisconnect();
    emit resultReady(s);
}



void WalletRpc::Balance::doWork() {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError();
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::Balance::operate, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, &WalletRpc::Balance::on_BalanceRetriveDone);
        connect(this, &WalletRpc::Balance::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit operate(Global::Network::getNodeAddress().second, WalletRpc::compose(1, "Balance", QList<QString>({Global::Account::getAccountPublicId()})));
}

void WalletRpc::Balance::on_BalanceRetriveDone(const QString &s) {
    emit socketDisconnect();
    emit resultReady(s);
}


void WalletRpc::Receive::doWork() {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyPublicId");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::Receive::operate, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, &WalletRpc::Receive::on_ReceiveRetriveDone);

        connect(this, &WalletRpc::Receive::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::Receive::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        id = 1;
        workerThread->start();
    }
    emit operate(Global::Network::getNodeAddress().second, WalletRpc::compose(id, "Receive", QList<QString>({Global::Account::getAccountPublicId()})));
}

void WalletRpc::Receive::on_ReceiveRetriveDone(const QString &s) {
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(s.toLocal8Bit());
    if(!jsonWalletFile.isObject()) {
        emit resultError("EmptyResponse");
        return;
    }
    QJsonObject jsonObject = jsonWalletFile.object();
    id = jsonObject["id"].toInt();
    if(!jsonObject["error"].isNull()) {
        emit socketDisconnect();
        QJsonObject jsonDataObject = jsonObject["error"].toObject();
        emit resultError(jsonDataObject["message"].toString());
    } else if(!jsonObject["method"].isNull()) {
        if(!jsonObject["method"].toString().compare("Sign")) {
            QJsonArray arrayObject = jsonObject["params"].toArray();
            if(!arrayObject.at(0).toString().compare("hash")) {
                QString hash = arrayObject.at(1).toString();
                //QString accId = arrayObject.at(2).toString();
                QString signature = signatures::getSignature(Global::Account::getAccountPrivateKey(), hash);
                emit sendMessage(WalletRpc::composeSign(id, signature));
            }
        }
    } else if(!jsonObject["result"].isNull()) {
        emit socketDisconnect();
        emit resultReady(s);
    }
}

void WalletRpc::Send::doWork(QString amount, QString destAccount, QString ticker) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::Send::operate, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, &WalletRpc::Send::on_SendRetriveDone);

        connect(this, &WalletRpc::Send::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::Send::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit operate(Global::Network::getNodeAddress().second, WalletRpc::compose(1, "Send", QList<QString>({Global::Account::getAccountPublicId(), amount, destAccount, ticker})));
}

void WalletRpc::Send::on_SendRetriveDone(const QString &s) {
    QJsonDocument jsonWalletFile = QJsonDocument::fromJson(s.toLocal8Bit());
    if(!jsonWalletFile.isObject()) {
        emit resultError("EmptyResponse");
        return;
    }
    QJsonObject jsonObject = jsonWalletFile.object();
    id = jsonObject["id"].toInt();
    if(!jsonObject["error"].isNull()) {
        emit socketDisconnect();
        QJsonObject jsonDataObject = jsonObject["error"].toObject();
        emit resultError(jsonDataObject["message"].toString());
    } else if(!jsonObject["method"].isNull()) {
        if(!jsonObject["method"].toString().compare("Sign")) {
            QJsonArray arrayObject = jsonObject["params"].toArray();
            if(!arrayObject.at(0).toString().compare("hash")) {
                QString hash = arrayObject.at(1).toString();
                //QString accId = arrayObject.at(2).toString();
                QString signature = signatures::getSignature(Global::Account::getAccountPrivateKey(), hash);
                emit sendMessage(WalletRpc::composeSign(id, signature));
            }
        }
    } else if(!jsonObject["result"].isNull()) {
        emit socketDisconnect();
        emit resultReady(s);
    }
}


