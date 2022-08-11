#include "rpcsocket.h"

#include <QtWebSockets/QWebSocket>
#include <QSslSocket>

#include "globallyra.h"

void RpcSocket::doWork(QString url, QString msg) {
    this->msg = msg;
    if(!WebSocket) {
        WebSocket = new QWebSocket;
        TimeoutTimer = new QTimer;
        WebSocket->ignoreSslErrors();
        connect(WebSocket, &QWebSocket::connected, this, [=]{
            Connected = true;
            qDebug() << "SSLWEBSOCKET 1 : Connected";
            WebSocket->sendTextMessage(msg);
            qDebug() << "SEND MSG: " << msg;
        });
        connect(WebSocket, &QWebSocket::disconnected, this, [=]{
            Connected = false;
            qDebug() << "SSLWEBSOCKET 2 :" << WebSocket->errorString();
            TimeoutTimer->stop();
        });
        connect(WebSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),this, [=] {
            // WARNING: Never ignore SSL errors in production code.
            // The proper way to handle self-signed certificates is to add a custom root
            // to the CA store.
            WebSocket->ignoreSslErrors();
            qDebug() << "SSLWEBSOCKET 3 : SSL Error";
        });
        connect(WebSocket, &QWebSocket::textMessageReceived, this, [=](const QString message) {
            TimeoutTimer->stop();
            qDebug() << "RECV MSG: " << message;
            emit resultReady(message);
        });
        TimeoutTimer->setInterval(RPC_CONNECTION_TIMEOUT_MS);
        connect(TimeoutTimer, &QTimer::timeout, this, [=]() {
            TimeoutTimer->stop();
            emit resultError("");
        });
    }
    if(!Connected) {
        qDebug() << "Thread running";
        WebSocket->open(QUrl(url));
    }
    TimeoutTimer->start();
}

void RpcSocket::sendMessage(QString message) {
    WebSocket->sendTextMessage(message);
    qDebug() << "SEND MSG: " << message;
}

void RpcSocket::socketDisconnect() {
    WebSocket->close();
}

/* Usage */

/*
void MyObject::startWorkInAThread()
{
    hirtoryRpcSocket = new RpcSocket("wss://173.212.228.110:4504/api/v1/socket",
                                     "{\"id\":1,\"jsonrpc\":\"2.0\",\"method\":\"History\",\"params\":[\"LF6LpgcrANQWrErPcLREAbKzJg9DLeuXXa45cz5hKsUng7aJ2zCrAgHbtkSXv5dXiEfUB8ypN8i3daUkmiJwcX8cbXSv5U\",\"0\",\"1639860329843\",\"0\"]}", this);
    connect(hirtoryRpcSocket, &RpcSocket::resultReady, this, &PageAccount::on_HistoryRetriveDone);
    connect(hirtoryRpcSocket, &RpcSocket::finished, this, &QObject::deleteLater);
    hirtoryRpcSocket->start();
}
*/
