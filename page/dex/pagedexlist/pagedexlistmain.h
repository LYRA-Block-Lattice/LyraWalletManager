#ifndef PAGEDEXLISTMAIN_H
#define PAGEDEXLISTMAIN_H

#include <QWidget>
#include <QList>

#include <ui_pagedex.h>
#include "pagedexlist.h"

namespace Ui {
class PageDexListMain;
}

class PageDexListMain : public QWidget
{
    Q_OBJECT

public:
    typedef PageDexList::entry_t entry_t;
    explicit PageDexListMain(QWidget *parent = nullptr);
    explicit PageDexListMain(QList<entry_t> entrys, QWidget *parent = nullptr);
    ~PageDexListMain();
    void append(QString ticker, QString tokenName, double spotAmount, double dexAmount);
    void insert(int index, QString ticker, QString tokenName, double spotAmount, double dexAmount);
    void clear();
    void removeAt(int index);
    qsizetype count(){ return EntryList.count(); }
    void repaint();
    QList<PageDexList *> entryList() { return EntryList;}
    void setWithdrawEnable(int index, bool enable);
    void setScale();
    void setStyle();

private:
    Ui::PageDexListMain *ui;

    QList<PageDexList *> EntryList;
    void renumerotateEntrys();

    QRect thisQRectBack;
signals:
    void setupPushButton(int index);
    void spotToDexPushButton(int index);
    void dexToSpotPushButton(int index);
    void depositPushButton(int index);
    void withdrawPushButton(int index);
};

#endif // PAGEDEXLISTMAIN_H
