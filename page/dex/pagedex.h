#ifndef PAGEDEX_H
#define PAGEDEX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QRect>
#include <QFont>
#include <QSize>

#include "api/web/webget.h"
#include "api/web/webclass.h"

#include "wallet/walletrpc.h"

#include "pagedexlist/pagedexlistmain.h"

namespace Ui {
class PageDex;
}

class PageDex : public QWidget
{
    Q_OBJECT

public:
    explicit PageDex(QWidget *parent = nullptr);
    ~PageDex();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private:
    Ui::PageDex *ui;
    QWidget *Parent;
    PageDexListMain *page;

    QMovie *progressMovie;
    QLabel *progressLabel;

    void dexGetSupportedTokens();
    void dexGetAllWalles();

    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;
    QRect progressMovieQRectBack;

    WebGet *dexGetSupportedTokensFetchWorker = nullptr;
    QThread *dexGetSupportedTokensFetchWorkerThread = nullptr;
    WebGet *dexGetAllWalletsFetchWorker = nullptr;
    QThread *dexGetAllWalletsFetchWorkerThread = nullptr;

    int AccountListChangedCount = -1;
    int SelectedAccountChangedCount = -1;
    networkName_e network = networkName_e::NONE;
    QList<WebClass::DexGetSuportedExtTokens::entry_t> EntryList;
    QList<WebClass::DexGetAllWallets::entry_t> WalletsEntryList;
signals:
    void dexGetSupportedTokensStart(QString url);
    void dexGetAllWalletsStart(QString url);
};

#endif // PAGEDEX_H
