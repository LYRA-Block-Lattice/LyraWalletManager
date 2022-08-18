#ifndef PAGESTAKING_H
#define PAGESTAKING_H

#include <QWidget>

#include <QRect>
#include <QFont>
#include <QSize>
#include <QMovie>
#include <QLabel>
#include <QTimer>

#include "pagestackinglist/pagestakinglist.h"

#include "wallet/walletrpc.h"

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
    PageStakingList *StakingList;
    QWidget *Parent;

    void fetchGetBrokerAccounts();
    void startAddStaking(double amount);
    void startUnstake();

    QMovie *progressMovie;
    QLabel *progressLabel;
    QTimer fadeTimer;
    int fadeCount = 0;
    QTimer delayGetBrokerAccounts;

    WalletRpc::GetBrokerAccounts *getBrokerAccountsThread = nullptr;
    QThread *getBrokerAccountsWorkerThread = nullptr;
    WalletRpc::AddStaking *addStakingThread = nullptr;
    QThread *addStakingsWorkerThread = nullptr;
    WalletRpc::UnStaking *unStakingThread = nullptr;
    QThread *unStakingsWorkerThread = nullptr;

    QRect HeaderFrameQRectBack;
    QRect TitleLabelQRectBack;
    QFont TitleLabelQFontBack;

    QRect StakePushButtonQRectBack;
    QFont StakePushButtonQFontBack;
    QRect UnstakePushButtonQRectBack;
    QFont UnstakePushButtonQFontBack;
    QRect AddStakingAccountPushButtonQRectBack;
    QFont AddStakingAccountPushButtonQFontBack;
    QRect ProgressMovieQRectBack;

    QRect statusLabelQRectBack;
    QFont statusLabelQFontBack;

    int AccountListChangedCount = -1;

signals:
    void getBrokerAccountsStartFetch();
    void addStakingStartFetch(QString stakingAccountId, double amount);
    void unStakingStartFetch(QString stakingAccountId);
private slots:
    void on_stakePushButton_clicked();
    void on_unstakePushButton_clicked();
    void on_addStakingAccountPushButton_clicked();
};

#endif // PAGESTAKING_H
