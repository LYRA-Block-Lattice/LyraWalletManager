#ifndef PAGESTAKINGLIST_H
#define PAGESTAKINGLIST_H

#include <QListView>

class PageStakingList : public QListView {
    Q_OBJECT
public:
    typedef struct {
        QString accountName;
        QString stakingAccountId;
        QString profitingAccountId;
        QString expiryDate;
        QString accountValidity;
        QString amount;
        bool status;
    }item_t;
    explicit PageStakingList(QWidget *parent = nullptr);
    void setScale();
    void setStyle();
    QString getName(int index);
    QString getStkId(int index);
    QString getVoting(int index);
    QList<QString> getAccounts();
public slots:
    void addMessage(PageStakingList::item_t arg);
    void setTokenValue(QString ticker, double tokenValue);
    void clearAll();
signals:
    void itemClicked(int item);
};

#endif // PAGEACCOUNTLIST_H
