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
        void doWork();
    signals:
        void startFetch(QString url, QString message);
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
        void doWork();
    signals:
        void startFetch(QString url, QString message);
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
        void doWork();
    signals:
        void startFetch(QString url, QString message);
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
        void doWork(QString amount, QString destAccount, QString ticker);
    signals:
        void startFetch(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };

    class Pool : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;
        QList<QString> userData;
    public slots:
        void doWork(QString token1, QString token0, QList<QString> *userData = nullptr);
    signals:
        void startFetch(QString url, QString message);
        void socketDisconnect();
        void resultReady(const QString &s, QList<QString> userData);
        void resultError();
    };

    class PoolCalculate : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;
    public slots:
        void doWork(QString poolId, QString swapFrom, double amount, double slippage);
    signals:
        void startFetch(QString url, QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError();
    };

    class Swap : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;

        QString Amount;
        QString DestAccount;
        QString Ticker;
        int id = 1;
    public slots:
        void doWork(QString token0, QString token1, QString tokenToSwap, double amountToSwap, double amountToGet);
    signals:
        void startFetch(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };

    class CreatePool : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;

        QString Amount;
        QString DestAccount;
        QString Ticker;
        int id = 1;
    public slots:
        void doWork(QString token0, QString token1);
    signals:
        void startFetch(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };

    class AddLiquidity : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;

        QString Amount;
        QString DestAccount;
        QString Ticker;
        int id = 1;
    public slots:
        void doWork(QString token0, double token0Amount, QString token1, double token1Amount);
    signals:
        void startFetch(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };

    class RemoveLiquidity : public QObject {
        Q_OBJECT
    private:
        RpcSocket *worker = nullptr;
        QThread *workerThread = nullptr;

        QString Amount;
        QString DestAccount;
        QString Ticker;
        int id = 1;
    public slots:
        void doWork(QString token0, QString token1);
    signals:
        void startFetch(QString url, QString message);
        void sendMessage(QString message);
        void socketDisconnect();
        void resultReady(const QString &s);
        void resultError(const QString &s);
    };

};

#endif // WALLETRPC_H
