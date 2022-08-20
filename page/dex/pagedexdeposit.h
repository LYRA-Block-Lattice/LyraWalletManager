#ifndef PAGEDEXDEPOSIT_H
#define PAGEDEXDEPOSIT_H

#include <QWidget>

namespace Ui {
class PageDexDeposit;
}

class PageDexDeposit : public QWidget
{
    Q_OBJECT

public:
    explicit PageDexDeposit(QWidget *parent = nullptr);
    ~PageDexDeposit();

private:
    Ui::PageDexDeposit *ui;
};

#endif // PAGEDEXDEPOSIT_H
