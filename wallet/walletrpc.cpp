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
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "History", QList<QString>({
                                                                     Global::Account::getAccountPublicId(),
                                                                     "0",
                                                                     QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                                     "0"
                                                                 })));
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
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "Balance", QList<QString>({
                                                                     Global::Account::getAccountPublicId()
                                                                 })));
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
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(id, "Receive", QList<QString>({
                                                                      Global::Account::getAccountPublicId()
                                                                  })));
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
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "Send", QList<QString>({
                                                                  Global::Account::getAccountPublicId(),
                                                                  amount,
                                                                  destAccount,
                                                                  ticker
                                                              })));
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
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "Pool", QList<QString>({
                                                                  token0,
                                                                  token1
                                                              })));
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
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "PoolCalculate", QList<QString>({
                                                                           poolId,
                                                                           swapFrom,
                                                                           QString::number(amount),
                                                                           QString::number(slippage)
                                                                       })));
}

void WalletRpc::Swap::doWork(QString token0, QString token1, QString tokenToSwap, double amountToSwap, double amountToGet) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::Swap::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::Swap::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::Swap::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "Swap", QList<QString>({
                                                                  Global::Account::getAccountPublicId(),
                                                                  token0,
                                                                  token1,
                                                                  tokenToSwap,
                                                                  QString::number(amountToSwap),
                                                                  QString::number(amountToGet)
                                                              })));
}

void WalletRpc::CreatePool::doWork(QString token0, QString token1) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::CreatePool::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::CreatePool::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::CreatePool::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "CreatePool", QList<QString>({
                                                                      Global::Account::getAccountPublicId(),
                                                                      token0,
                                                                      token1
                                                                    })));
}

void WalletRpc::AddLiquidity::doWork(QString token0, double token0Amount, QString token1, double token1Amount) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::AddLiquidity::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::AddLiquidity::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::AddLiquidity::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "AddLiquidaty", QList<QString>({
                                                                        Global::Account::getAccountPublicId(),
                                                                        token0,
                                                                        QString::number(token0Amount),
                                                                        token1,
                                                                        QString::number(token1Amount)
                                                                    })));
}

void WalletRpc::RemoveLiquidity::doWork(QString token0, QString token1) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::RemoveLiquidity::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::RemoveLiquidity::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::RemoveLiquidity::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "RemoveLiquidaty", QList<QString>({
                                                                        Global::Account::getAccountPublicId(),
                                                                        token0,
                                                                        token1,
                                                                    })));
}

void WalletRpc::GetBrokerAccounts::doWork() {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError();
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::GetBrokerAccounts::startFetch, worker, &RpcSocket::doWork);
        connect(worker, &RpcSocket::resultReady, this, [=](const QString s) {
            emit socketDisconnect();
            emit resultReady(s);
            this->thread()->exit();
        });
        connect(this, &WalletRpc::GetBrokerAccounts::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "GetBrokerAccounts", QList<QString>({
                                                                     Global::Account::getAccountPublicId()
                                                                 })));
}

void WalletRpc::AddStaking::doWork(QString stakingAccountId, double amount) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::AddStaking::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::AddStaking::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::AddStaking::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "AddStaking", QList<QString>({
                                                                        Global::Account::getAccountPublicId(),
                                                                        stakingAccountId,
                                                                        QString::number(amount)
                                                                    })));
}

void WalletRpc::UnStaking::doWork(QString stakingAccountId) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::UnStaking::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::UnStaking::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::UnStaking::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "UnStaking", QList<QString>({
                                                                        Global::Account::getAccountPublicId(),
                                                                        stakingAccountId
                                                                    })));
}

void WalletRpc::CreateStakingAccount::doWork(QString name, QString voteFor, int days) {
    if(Global::Account::getAccountPublicId().length() == 0) {
        emit resultError("EmptyResponse");
        return;
    }
    if(!worker) {
        worker = new RpcSocket;
        workerThread= new QThread;
        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &WalletRpc::CreateStakingAccount::startFetch, worker, &RpcSocket::doWork);
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
        connect(this, &WalletRpc::CreateStakingAccount::sendMessage, worker, &RpcSocket::sendMessage);
        connect(this, &WalletRpc::CreateStakingAccount::socketDisconnect, worker, &RpcSocket::socketDisconnect);
        workerThread->start();
    }
    emit startFetch("wss://" + Global::Network::getNodeAddress().second + LYRA_RPC_API_URL,
                 WalletRpc::compose(1, "CreateStakingAccount", QList<QString>({
                                                                                Global::Account::getAccountPublicId(),
                                                                                name,
                                                                                voteFor,
                                                                                QString::number(days),
                                                                                "true"
                                                                                })));
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


