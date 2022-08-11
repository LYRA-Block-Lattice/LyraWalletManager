#include "webget.h"
#include "QtCore/qthread.h"

#include <QtWebSockets/QWebSocket>
#include <QSslSocket>

void WebGet::doWork(QString url) {
    manager = new QNetworkAccessManager;
    connect(manager, &QNetworkAccessManager::finished, this,
            [=] {
        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if(status_code.isValid()){
            // Print or catch the status code
            qDebug() << "WEB GET 1: " << status_code.toString(); // or status_code.toInt();
        }
    });
    reply = manager->get(QNetworkRequest(QUrl(url)));
    connect(reply, &QNetworkReply::readyRead, this,
            [this] {
            read.append(reply->readAll());
        }
    );
    connect(reply, &QNetworkReply::finished,
            this, [=] {
        emit resultReady(read);
        read.clear();
        timeoutTimer->stop();
        this->thread()->exit();
    });
    connect(reply, &QNetworkReply::errorOccurred,
            this, [=] {
        emit resultError("WEB GET 2: " + QString::number(reply->error()));
        timeoutTimer->stop();
        this->thread()->exit();
    });

    timeoutTimer = new QTimer();
    timeoutTimer->setInterval(5000);
    connect(timeoutTimer, &QTimer::timeout, this, [=]{
        timeoutTimer->stop();
        emit resultError("WEB GET 3: Timeout");
        this->thread()->exit();
    });
    //qDebug() << m_webGet->errorString();
    timeoutTimer->start();
    qDebug() << url;
}

/*
void MyObject::startWorkInAThread() {
    workerThread = new WebGet;
    thread = new QThread;
    workerThread->moveToThread(thread);
    connect(workerThread, &WebGet::resultReady, this, &MainWindow::on_coingeckoFetchDone);
    connect(workerThread, &WebGet::resultError, this, &MainWindow::on_coingeckoFetchError);
    connect(thread, &QThread::finished, workerThread, &QObject::deleteLater);
    connect(this, &MainWindow::fetchCoinGeckoSignal, workerThread, &WebGet::doWork);
    thread->start();
    emit fetchCoinGeckoSignal("https://someSite.com");
}
*/
