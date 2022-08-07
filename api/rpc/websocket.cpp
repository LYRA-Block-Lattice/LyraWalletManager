#include "websocket.h"

WebSocket::WebSocket(QString url, QObject *parent) {
    setParent(parent);
    this->url = url;
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocket::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocket::closed);
    timeoutTimer.setInterval(10000);
    connect(&timeoutTimer, &QTimer::timeout, this, &WebSocket::onResponseTimeout);
}

void WebSocket::run() {
    m_webSocket.open(QUrl(url));
    timeoutTimer.start();
}

void WebSocket::onConnected() {
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocket::onReceived);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void WebSocket::onReceived(QString message) {
    timeoutTimer.stop();
    emit resultReady(message);
}

void WebSocket::onResponseTimeout() {
    timeoutTimer.stop();
    m_webSocket.close();
    emit resultReady("");
}

/*
void MyObject::startWorkInAThread()
{
    WebSocket *workerThread = new WebSocket("https://some_url, this);
    connect(webSocket, &WebSocket::resultReady, this, &MyObject::handleResults);
    connect(webSocket, &WebSocket::finished, workerThread, &QObject::deleteLater);
    webSocket->start();
}
*/
