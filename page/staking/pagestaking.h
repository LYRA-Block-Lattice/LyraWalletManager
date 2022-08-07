#ifndef PAGESTAKING_H
#define PAGESTAKING_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>

namespace Ui {
class PageStaking;
}

class PageStaking : public QWidget
{
    Q_OBJECT

public:
    explicit PageStaking(QWidget *parent = nullptr);
    ~PageStaking();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private:
    Ui::PageStaking *ui;

    QRect headerFrameQRectBack;

    int AccountListChangedCount = -1;

};

#endif // PAGESTAKING_H
