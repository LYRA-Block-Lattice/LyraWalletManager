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
        connect(this, &WalletRpc::History::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            emit socketDisconnect();
            emit resultReady(s);
            this->thread()->exit();
        });
        connect(this, &WalletRpc::History::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch(Global::Network::getNodeAddress().second,
                 WalletRpc::compose(1, "History", QList<QString>({Global::Account::getAccountPublicId(), "0", QString::number(QDateTime::currentMSecsSinceEpoch()), "0"})));
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
        connect(this, &WalletRpc::Balance::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            emit socketDisconnect();
            emit resultReady(s);
            this->thread()->exit();
        });
        connect(this, &WalletRpc::Balance::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch(Global::Network::getNodeAddress().second,
                 WalletRpc::compose(1, "Balance", QList<QString>({Global::Account::getAccountPublicId()})));
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
        connect(this, &WalletRpc::Receive::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            QJsonDocument jsonWalletFile = QJsonDocument::fromJson(s.toLocal8Bit());
            if(!jsonWalletFile.isObject()) {
                emit resultError("EmptyResponse");
                this->thread()->exit();
                return;
            }
            QJsonObject jsonObject = jsonWalletFile.object();
            id = jsonObject["id"].toInt();
            if(!jsonObject["error"].isNull()) {
                emit socketDisconnect();
                QJsonObject jsonDataObject = jsonObject["error"].toObject();
                emit resultError(jsonDataObject["message"].toString());
                this->thread()->exit();
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
                this->thread()->exit();
            }
        });
        connect(this, &WalletRpc::Receive::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::Receive::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        id = 1;
        workerThread->start();
    }
    emit startFetch(Global::Network::getNodeAddress().second,
                 WalletRpc::compose(id, "Receive", QList<QString>({Global::Account::getAccountPublicId()})));
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
        connect(this, &WalletRpc::Send::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            QJsonDocument jsonWalletFile = QJsonDocument::fromJson(s.toLocal8Bit());
            if(!jsonWalletFile.isObject()) {
                emit resultError("EmptyResponse");
                this->thread()->exit();
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
                this->thread()->exit();
            }
        });
        connect(this, &WalletRpc::Send::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::Send::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch(Global::Network::getNodeAddress().second,
                 WalletRpc::compose(1, "Send", QList<QString>({Global::Account::getAccountPublicId(), amount, destAccount, ticker})));
}

void WalletRpc::Pool::doWork(QString token1, QString token0, QList<QString> *userData) {
    if(userData)
        this->userData = *userData;
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::Pool::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            emit socketDisconnect();
            emit resultReady(s, this->userData);
            this->thread()->exit();
        });
        connect(this, &WalletRpc::Pool::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch(Global::Network::getNodeAddress().second,
                 WalletRpc::compose(1, "Pool", QList<QString>({token0, token1})));
}

void WalletRpc::PoolCalculate::doWork(QString poolId, QString swapFrom, double amount, double slippage) {
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::PoolCalculate::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            emit socketDisconnect();
            emit resultReady(s);
            this->thread()->exit();
        });
        connect(this, &WalletRpc::PoolCalculate::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch(Global::Network::getNodeAddress().second,
                 WalletRpc::compose(1, "PoolCalculate", QList<QString>({poolId, swapFrom, QString::number(amount), QString::number(slippage)})));
}

/*
    historyThread = new WalletRpc::History;
    historyWorkerThread = new QThread;
    historyThread->moveToThread(historyWorkerThread);
    connect(historyWorkerThread, &QThread::finished, historyThread, &QObject::deleteLater);
    connect(this, &PageAccount::historyOperate, historyThread, &WalletRpc::History::doWork);
    connect(historyThread, &WalletRpc::History::resultReady, this, &PageAccount::on_HistoryRetriveDone);
    connect(historyThread, &WalletRpc::History::resultError, this, &PageAccount::on_HistoryRetriveError);
    historyWorkerThread->start();
    emit historyOperate();

 */


