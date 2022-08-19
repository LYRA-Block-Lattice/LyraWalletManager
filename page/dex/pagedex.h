#ifndef PAGEDEX_H
#define PAGEDEX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QRect>
#include <QFont>
#include <QSize>

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

    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;


    int AccountListChangedCount = -1;
};

#endif // PAGEDEX_H
