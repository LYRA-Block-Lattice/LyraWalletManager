#ifndef PAGESTAKINGADDACCOUNTLIST_H
#define PAGESTAKINGADDACCOUNTLIST_H

#include <QListView>

class PageStakingAddAccountList : public QListView {
    Q_OBJECT
public:
    typedef struct {
        QString accountName;
        QString accountType;
        QString shareRatio;
        QString seats;
        QString timeStamp;
        QString totalProfit;
        QString totalStaked;
        QString yourShare;
        bool fetch;
        QString pftId;
    } item_t;
    explicit PageStakingAddAccountList(QWidget *parent = nullptr);
    void setScale();
    void setStyle();
    QString getName(int index);
    bool getFetch(int index);
    QString getPftId(int index);
    double getTotalStaked(int index);
    double getShareRatio(int index);
    QList<QString> getAccounts();
public slots:
    void addMessage(PageStakingAddAccountList::item_t arg);
    void setFetch(int index, double totalStaked, double yourShare);
    void clearAll();
signals:
    void itemClicked(int item);
};

#endif // PAGEACCOUNTLIST_H
