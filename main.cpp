#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QWindow>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    qDebug() << uiLanguages;
    for (const QString &locale : uiLanguages) {
        const QString baseName = "LyraPcWalletManager_" + QLocale(locale).name();
        if (translator.load(":/languages/" + baseName)) {
            a.installTranslator(&translator);
            qDebug() << ":/languages/" + baseName;
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
