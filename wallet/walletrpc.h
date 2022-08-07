#ifndef WALLETRPC_H
#define WALLETRPC_H

#include <QObject>
#include <QThread>

#include "api/rpc/rpcsocket.h"
#include "storage/storageinternal.h"


namespace WalletRpc {
static QString compose(int id, QString method, QList<QString> arg);
static QString composeSign(int id, QString sig);

    class History : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;
    public slots:
        void on_HistoryRetriveDone(const QString &s);
        void doWork();
    signals:
        void operate(QString url, QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError();
    };

    class Balance : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;
    public slots:
        void on_BalanceRetriveDone(const QString &s);
        void doWork();
    signals:
        void operate(QString url, QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError();
    };

    class Receive: public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;
        int id = 1;
    public slots:
        void on_ReceiveRetriveDone(const QString &s);
        void doWork();
    signals:
        void operate(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };

    class Send : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;

        QString Amount;
        QString DestAccount;
        QString Ticker;
        int id = 1;
    public slots:
        void on_SendRetriveDone(const QString &s);
        void doWork(QString amount, QString destAccount, QString ticker);
    signals:
        void operate(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };
};

#endif // WALLETRPC_H
