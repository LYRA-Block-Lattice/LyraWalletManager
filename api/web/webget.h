#ifndef WEBNETWORK_H
#define WEBNETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QTimer>
#include <QEventLoop>

class WebGet : public QObject {
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString read;
    QTimer *timeoutTimer;
signals:
    void resultReady(const QString &s);
    void resultError(const QString &s);
public slots:
    void doWork(QString url);
};

#endif // WEBNETWORK_H
