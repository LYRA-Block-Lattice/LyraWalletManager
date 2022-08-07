#ifndef PAGEACCOUNTLIST_H
#define PAGEACCOUNTLIST_H

#include <QListView>

class PageAccountList : public QListView
{
public:
    Q_OBJECT
public:
    explicit PageAccountList(QWidget *parent = nullptr);
    void setScale();
    void setStyle();

public slots:
    void addMessage(const QPixmap &pixmap, const QString &ticker, const double &amount, const double &value, const double &tokenValue);
    void clearAll();
};

#endif // PAGEACCOUNTLIST_H
