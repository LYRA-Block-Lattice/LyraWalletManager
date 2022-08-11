#ifndef RPCNETWORK_H
#define RPCNETWORK_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QSslError>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QTimer>
#include <QEventLoop>

class RpcSocket : public QObject {
    Q_OBJECT
public:
private:
    QString Url;
    QString msg;
    QWebSocket *WebSocket = nullptr;
    QTimer *TimeoutTimer;
    bool Connected = false;
signals:
    void resultReady(const QString &s);
    void resultError(const QString &s);
public slots:
    void doWork(QString url, QString msg);
    void sendMessage(QString message);
    void socketDisconnect();
};

#endif // RPCNETWORK_H
