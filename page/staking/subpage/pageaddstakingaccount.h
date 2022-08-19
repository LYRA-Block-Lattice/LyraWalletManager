#ifndef PAGEADDSTAKINGACCOUNT_H
#define PAGEADDSTAKINGACCOUNT_H

#include <QWidget>
#include <QLabel>

#include "api/rpc/rpcclass.h"
#include "extensions/QZXing/QZXing.h"

#include "api/web/webget.h"
#include "api/web/webclass.h"

#include "wallet/walletrpc.h"

#include "pageaddstakingaccountlist/pagestakingaddaccountlist.h"

namespace Ui {
class PageAddStakingAccount;
}

class PageAddStakingAccount : public QWidget {
    Q_OBJECT

public:
    explicit PageAddStakingAccount(QWidget *parent = nullptr);
    ~PageAddStakingAccount();
    void open();
    void close();
    void setScale();
    void setStyle();
    void loop();

private slots:
    void on_backPushButton_clicked();
    void on_amountLineEdit_textChanged(const QString &arg1);
    void on_maxPushButton_clicked();
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_daysLineEdit_textChanged(const QString &arg1);
    void on_previewPushButton_clicked();

private:
    void findAllProfitingAccounts();
    void findAllStakings(int index);
    void createStakingAccount();
    void startAddStaking(QString stkId);

    Ui::PageAddStakingAccount *ui;

    PageStakingAddAccountList *StakingList;

    WebGet *allProfitingAccountsFetchWorker = nullptr;
    QThread *allProfitingAccountsFetchWorkerThread = nullptr;
    WebGet *allStakingsFetchWorker = nullptr;
    QThread *allStakingsFetchWorkerThread = nullptr;

    WalletRpc::CreateStakingAccount *createStakingAccountThread = nullptr;
    QThread *createStakingAccountWorkerThread = nullptr;
    WalletRpc::AddStaking *addStakingThread = nullptr;
    QThread *addStakingsWorkerThread = nullptr;

    QMovie *progressMovie;
    QLabel *progressLabel;
    QTimer delayGoStakingPage;

    QRect backPushButtonQRectBack;
    QSize backPushButtonQSizeBack;
    QRect headerFrameQRectBack;
    QRect titleLabelQRectBack;
    QFont titleLabelQFontBack;


    QRect AmountGroupBoxQRectBack;
    QRect AmountLineEditQRectBack;
    QFont AmountLineEditQFontBack;
    QRect MaxPushButtonQRectBack;
    QSize MaxPushButtonQSizeBack;
    QRect AvailablePushButtonQRectBack;
    QFont AvailablePushButtonQFontBack;

    QRect StakeForGroupBoxQRectBack;

    QRect NameDaysGroupBoxQRectBack;
    QRect NameLineEditQRectBack;
    QFont NameLineEditQFontBack;
    QRect DaysLineEditQRectBack;
    QFont DaysLineEditQFontBack;
    QRect PreviewPushButtonQRectBack;
    QFont PreviewPushButtonQFontBack;

    QRect ProgressMovieQRectBack;
    QRect statusLabelQRectBack;
    QFont statusLabelQFontBack;

signals:
    void allProfitingAccountsFetch(QString url);
    void allStakingsFetch(QString url);
    void createStakingAccountStart(QString name, QString voteFor, int days);
    void addStakingStart(QString stakingAccountId, double amount);
};

#endif // PAGEADDSTAKINGACCOUNT_H
