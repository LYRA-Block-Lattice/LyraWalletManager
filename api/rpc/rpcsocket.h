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
    QString Message;
    QWebSocket *WebSocket = nullptr;
    QTimer *TimeoutTimer;
    bool Connected = false;
signals:
    void resultReady(const QString &s);
private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);
public slots:
    void doWork(QString url, QString message);
    void onResponseTimeout();
    void sendMessage(QString message);
    void socketDisconnect();
};

#endif // RPCNETWORK_H
