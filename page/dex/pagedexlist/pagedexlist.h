#ifndef PAGEDEXLIST_H
#define PAGEDEXLIST_H

#include <QWidget>
#include <QPixmap>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageDexList;
}

class PageDexList : public QWidget
{
    Q_OBJECT

public:
    typedef struct {
        int Index;
        QString Ticker;
        QString TokenName;
        double SpotAmount;
        double DexAmount;
    }entry_t;
    explicit PageDexList(entry_t entry, QWidget *parent = nullptr);
    ~PageDexList();
    void setIndex(int index) { Index = index;}
    void setTicker(QString ticker);
    void setTokenName(QString tokenName);
    QString ticker() { return Ticker;}
    void setSpotAmount(double amount);
    double spotAmount() { return SpotAmount;}
    void setDexAmount(double amount);
    double dexAmount() { return DexAmount;}
    void setWithdrawEnable( bool enable);
    void setScale();
    void setStyle();

private:
    Ui::PageDexList *ui;
    QWidget *Parent;
    int Index;
    QString Ticker;
    QString TokenName;
    double SpotAmount;
    double DexAmount;

    QRect thisQRectBack;
    QRect groupBoxQRectBack;

    QRect tickerIconLabelQRectBack;
    QSize tickerIconLabelQSizeBack;

    QRect tickerTextLabelQRectBack;
    QFont tickerTextLabelQFontBack;
    QRect labelQRectBack;
    QFont labelQFontBack;
    QRect spotLabelQRectBack;
    QFont spotLabelQFontBack;
    QRect dexLabelQRectBack;
    QFont dexLabelQFontBack;
    QRect spotAmountLabelQRectBack;
    QFont spotAmountLabelQFontBack;
    QRect dexAmountLabelQRectBack;
    QFont dexAmountLabelQFontBack;

    QRect setupPushButtonQRectBack;
    QSize setupPushButtonQSizeBack;
    QRect spotToDexPushButtonQRectBack;
    QSize spotToDexPushButtonQSizeBack;
    QRect dexToSpotPushButtonQRectBack;
    QSize dexToSpotPushButtonQSizeBack;

    QRect depositPushButtonQRectBack;
    QFont depositPushButtonQFontBack;
    QRect withdrawPushButtonQRectBack;
    QFont withdrawPushButtonQFontBack;
signals:
    void setupPushButton(int index);
    void spotToDexPushButton(int index);
    void dexToSpotPushButton(int index);
    void depositPushButton(int index);
    void withdrawPushButton(int index);
private slots:
    void on_setupPushButton_clicked();
    void on_spotToDexPushButton_clicked();
    void on_dexToSpotPushButton_clicked();
    void on_depositPushButton_clicked();
    void on_withdrawPushButton_clicked();
};

#endif // PAGEDEXLIST_H
