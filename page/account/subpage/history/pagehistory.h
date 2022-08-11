#ifndef PAGEHISTORY_H
#define PAGEHISTORY_H

#include <QWidget>
#include <QRect>
#include <QFont>
#include <QSize>
#include <QMovie>
#include <QLabel>
#include <QProgressBar>

#include "pagehistorylist/pagehistorylist.h"
#include "wallet/walletrpc.h"

namespace Ui {
class PageHistory;
}

class PageHistory : public QWidget {
    Q_OBJECT

public:
    explicit PageHistory(QWidget *parent = nullptr);
    ~PageHistory();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();


private slots:
    void on_ItemSelected(int item);
    void on_backPushButton_clicked();

private:
    Ui::PageHistory *ui;
    QWidget *parent;
    PageHistoryList *messageList;
    QProgressBar *historyLoadProgressBar;

    QString listHash;

    QList<RpcClass::History::entry_t> tmpListEntrys;
    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;
    QRect messageListQRectBack;
    QRect historyLoadProgressBarQRectBack;

    int AccountListChangedCount = -1;
    int historyChangeCount = -1;
    int BalancesChangedCount = -1;
    QList<QPair<int, QPair<QString, double>>> tokenPriceList;

    void showProgress(bool show);
};

#endif // HISTORY_H
