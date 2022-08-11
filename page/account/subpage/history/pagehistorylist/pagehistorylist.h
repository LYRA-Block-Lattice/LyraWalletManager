#ifndef PAGEHISTORYLIST_H
#define PAGEHISTORYLIST_H

#include <QListView>

class PageHistoryList : public QListView
{
public:
    Q_OBJECT
public:
    explicit PageHistoryList(QWidget *parent = nullptr);
    void setScale();
    void setStyle();


public slots:
    void addMessage(const QPixmap &pixmap, const QString &ticker, const double &amount, const double &value, const double &tokenValue, const qint64 &height);
    void setTokenValue(QString ticker, double tokenValue);
    QList<QString> getTickers();
    void clearAll();
    void onListClicked(const QModelIndex index);
signals:
    void itemClicked(int item);
};

#endif // PAGEACCOUNTLIST_H
