#ifndef WEBNETWORK_H
#define WEBNETWORK_H

#include <QObject>
#include <QThread>
#include <QWebSocket>
#include <QTimer>

class WebSocket : public QThread {
    Q_OBJECT
private:
    QString url;
    QWebSocket m_webSocket;
    QTimer timeoutTimer;
public:
    WebSocket(QString url, QObject *parent = nullptr);
    void run() override;
signals:
    void resultReady(const QString &s);
Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onReceived(QString message);
public slots:
    void onResponseTimeout();
};

#endif // WEBNETWORK_H
