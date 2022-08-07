#include "rpcsocket.h"

#include <QtWebSockets/QWebSocket>
#include <QSslSocket>

#include "globallyra.h"

void RpcSocket::doWork(QString url, QString message) {
    this->Message = message;
    if(!WebSocket) {
        WebSocket = new QWebSocket;
        TimeoutTimer = new QTimer;
        WebSocket->ignoreSslErrors();
        connect(WebSocket, &QWebSocket::connected, this, &RpcSocket::onConnected);
        connect(WebSocket, &QWebSocket::disconnected, this, &RpcSocket::onDisconnected);
        connect(WebSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
                this, &RpcSocket::onSslErrors);
        connect(WebSocket, &QWebSocket::textMessageReceived,
                this, &RpcSocket::onTextMessageReceived);
        TimeoutTimer->setInterval(RPC_CONNECTION_TIMEOUT_MS);
        connect(TimeoutTimer, &QTimer::timeout, this, &RpcSocket::onResponseTimeout);
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

void RpcSocket::onConnected() {
    Connected = true;
    qDebug() << "SSLWEBSOCKET 1 : Connected";
    WebSocket->sendTextMessage(Message);
    qDebug() << "SEND MSG: " << Message;
}

void RpcSocket::onDisconnected() {
    Connected = false;
    qDebug() << "SSLWEBSOCKET 2 :" << WebSocket->errorString();
    TimeoutTimer->stop();
}

void RpcSocket::onTextMessageReceived(QString message) {
    TimeoutTimer->stop();
    qDebug() << "RECV MSG: " << message;
    emit resultReady(message);
}

void RpcSocket::onSslErrors(const QList<QSslError> &errors) {
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    WebSocket->ignoreSslErrors();
    qDebug() << "SSLWEBSOCKET 3 : SSL Error";
}

void RpcSocket::onResponseTimeout() {
    TimeoutTimer->stop();
    emit resultReady("");
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
