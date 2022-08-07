#ifndef PAGESWAP_H
#define PAGESWAP_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageSwap;
}

class PageSwap : public QWidget
{
    Q_OBJECT

public:
    explicit PageSwap(QWidget *parent = nullptr);
    ~PageSwap();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private:
    Ui::PageSwap *ui;

    QRect headerFrameQRectBack;

    int AccountListChangedCount = -1;

};

#endif // PAGESWAP_H
