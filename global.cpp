
#include "global.h"

#include "mainwindow.h"

QTabWidget *mainTabWidget;

PageStaking *pageStaking;
PageSwap *pageSwap;
PageAccount *pageAccount;
PageDex *pageDex;
PageMore *pageMore;

PageImportWallet *pageImportWallet;
PageNewAccount *pageNewAccount;
PageNewWallet *pageNewWallet;
PageOpenWallet *pageOpenWallet;
PageRecoverAccount *pageRecoverAccount;

PageHistory *pageHistory;
PageTransactionDetail *pageTransactionDetail;
PageSend *pageSend;
PageReceive *pageReceive;
PageAddStakingAccount *pageAddStakingAccount;

PageSettings *pageSettings;

/**********************************************************************************/
double yScale = 1.0;
double xScale = 1.0;
int ScreenWidth = 0;
int ScreenHeight = 0;
int headerHeight = 0.0;

/**********************************************************************************/
networkName_e Net = networkName_e::TESTNET;

QList<QList<QPair<QString,QString>>> NodeList = {
    {// Testnet
        QPair<QString,QString>("Seed1","seed.testnet.lyra.live:443"),
        QPair<QString,QString>("Seed2","seed2.testnet.lyra.live:443"),
        QPair<QString,QString>("Seed3","seed3.testnet.lyra.live:443"),
        QPair<QString,QString>("Seed4","seed4.testnet.lyra.live:443"),
        QPair<QString,QString>("Morgoth1","173.212.228.110:4504")
    }, { // Mainnet
        QPair<QString,QString>("Seed1","seed1.mainnet.lyra.live:443"),
        QPair<QString,QString>("Seed2","seed2.mainnet.lyra.live:443"),
        QPair<QString,QString>("Seed3","seed3.mainnet.lyra.live:443"),
        QPair<QString,QString>("Seed4","seed4.mainnet.lyra.live:443"),
        QPair<QString,QString>("Morgoth1","173.212.228.110:5504")
    }, { // Devnet
        QPair<QString,QString>("Seed1","173.212.228.110:443"),
        QPair<QString,QString>("Seed2","173.212.228.110:443"),
        QPair<QString,QString>("Seed3","173.212.228.110:443"),
        QPair<QString,QString>("Seed4","173.212.228.110:443"),
        QPair<QString,QString>("Morgoth1","173.212.228.110:3504")
    }
};

/**********************************************************************************/
Global::Page::PAGE accountManagerPage = Global::Page::OPEN_WALLET;

/*void Global::Page::backManagerPage() {
    pageImportWallet->setVisibility(false);
    pageNewAccount->setVisibility(false);
    pageNewWallet->setVisibility(false);
    pageOpenWallet->setVisibility(false);
    pageRecoverAccount->setVisibility(false);
    //pageHistory->setVisible(false);
    mainTabWidget->setVisible(true);
    if(accountManagerPage == Global::Page::IMPORT_WALLET) {
        mainTabWidget->setVisible(false);
        pageOpenWallet->setVisibility(true);
    } else if(accountManagerPage == Global::Page::NEW_ACCOUNT) {

    } else if(accountManagerPage == Global::Page::NEW_WALLET) {
        mainTabWidget->setVisible(false);
        pageOpenWallet->setVisibility(true);
    } else if(accountManagerPage == Global::Page::RECOVER_ACCOUNT) {

    } else if(accountManagerPage == Global::Page::HISTORY) {
        mainTabWidget->setVisible(true);
    } else if(accountManagerPage == Global::Page::TRANSACTION_DETAIL) {

    }
}*/

void Global::Page::goManagerPage(Global::Page::PAGE page, void *arg1) {
    pageImportWallet->setVisibility(false);
    pageNewAccount->setVisibility(false);
    pageNewWallet->setVisibility(false);
    pageOpenWallet->setVisibility(false);
    pageRecoverAccount->setVisibility(false);
    pageHistory->setVisible(false);
    pageTransactionDetail->setVisible(false);
    pageSend->setVisible(false);
    pageReceive->setVisible(false);
    pageSettings->setVisible(false);
    pageAddStakingAccount->setVisible(false);
    mainTabWidget->setVisible(false);
    switch(page) {
    case Global::Page::OPEN_WALLET:
        pageOpenWallet->setVisibility(true);
        pageOpenWallet->open();
        break;
    case Global::Page::IMPORT_WALLET:
        pageImportWallet->setVisibility(true);
        pageImportWallet->open();
        break;
    case Global::Page::NEW_ACCOUNT:
        pageNewAccount->setVisibility(true);
        pageNewAccount->open();
        break;
    case Global::Page::NEW_WALLET:
        pageNewWallet->setVisibility(true);
        pageNewWallet->open();
        break;
    case Global::Page::RECOVER_ACCOUNT:
        pageRecoverAccount->setVisibility(true);
        pageRecoverAccount->open();
        break;
    case Global::Page::HISTORY:
        pageHistory->setVisible(true);
        pageHistory->open();
        break;
    case Global::Page::TRANSACTION_DETAIL:
        pageTransactionDetail->setVisible(true);
        pageTransactionDetail->open((RpcClass::History::entry_t *) arg1);
        break;
    case Global::Page::SEND:
        pageSend->setVisible(true);
        pageSend->open();
        break;
    case Global::Page::RECEIVE:
        pageReceive->setVisible(true);
        pageReceive->open();
        break;
    case Global::Page::SETTINGS:
        pageSettings->setVisible(true);
        pageSettings->open();
        break;
    case Global::Page::ADD_STAKING_ACCOUNT:
        pageAddStakingAccount->setVisible(true);
        pageAddStakingAccount->open();
        break;
    default:
        mainTabWidget->setVisible(true);
        mainTabWidget->setCurrentIndex(page);
        break;
    }
    if(accountManagerPage != page) {
        switch(accountManagerPage) {
        case Global::Page::STAKING:
            pageStaking->close();
            break;
        case Global::Page::SWAP:
            pageSwap->close();
            break;
        case Global::Page::ACCOUNT:
            pageAccount->close();
            break;
        case Global::Page::DEX:
            pageDex->close();
            break;
        case Global::Page::MORE:
            pageMore->close();
            break;
        default:
            break;
        }
        switch(page) {
        case Global::Page::STAKING:
            pageStaking->open();
            break;
        case Global::Page::SWAP:
            pageSwap->open();
            break;
        case Global::Page::ACCOUNT:
            pageAccount->open();
            break;
        case Global::Page::DEX:
            pageDex->open();
            break;
        case Global::Page::MORE:
            pageMore->open();
            break;
        default:
            break;
        }
    }
    accountManagerPage = page;
    MainWindow::setPage(page);
}
/**********************************************************************************/
QString WalletName = "";
QString WalletPass = "";

void Global::Wallet::Name::set(QString name) {
    WalletName = name;
}

QString Global::Wallet::Name::get() {
    return WalletName;
}

void Global::Wallet::Password::set(QString p) {
    WalletPass = p;
}

QString Global::Wallet::Password::get() {
    return WalletPass;
}

bool Global::Wallet::Password::check(QString p) {
    return WalletPass.compare(p) ? false : true;
}

void Global::Wallet::Password::destroy() {
    WalletPass.clear();
}
/**********************************************************************************/
QList<Global::Account::account_t> AccountList;
int AccountListChangeCount = 0;

int SelectedAccount = -1;

bool Global::Account::setAccountList(QList<Global::Account::account_t> accountList) {
    AccountList.clear();
    AccountList.append(accountList);
    AccountListChangeCount++;
    return true;
}

void Global::Account::clearAccountList() {
    AccountList.clear();
    SelectedAccount = -1;
}

QList<Global::Account::account_t> Global::Account::getAccountList() {
    return AccountList;
}

Global::Account::account_t *Global::Account::getAccount() {
    if(SelectedAccount != -1 && SelectedAccount < Global::Account::getAccountList().count())
        return &AccountList[SelectedAccount];
    return nullptr;
}

QString Global::Account::getAccountName() {
    if(SelectedAccount != -1 && SelectedAccount < Global::Account::getAccountList().count())
        return AccountList[SelectedAccount].accountName;
    return "";
}

QString Global::Account::getAccountPrivateKey() {
    if(SelectedAccount != -1 && SelectedAccount < Global::Account::getAccountList().count())
        return AccountList[SelectedAccount].privateKey;
    return "";
}

QString Global::Account::getAccountPublicId() {
    if(SelectedAccount != -1 && SelectedAccount < Global::Account::getAccountList().count())
        return AccountList[SelectedAccount].publicId;
    return "";
}

void Global::Account::addAccount(Global::Account::account_t account) {
    if(AccountList.count() == 0)
        SelectedAccount = 0;
    AccountList.append(account);
    AccountListChangeCount++;
    if(SelectedAccount == -1)
        SelectedAccount = 0;
}

void Global::Account::addAccount(QString accountName, QString privateKey, QString publicId) {
    Global::Account::account_t account;
    account.accountName = accountName;
    account.privateKey = privateKey;
    account.publicId = publicId;
    addAccount(account);
}

void Global::Account::delAccount(QString name) {
    for (int i = 0; i < AccountList.count(); i++) {
        if (!AccountList[i].accountName.compare(name)) {
            AccountList.removeAt(i);
            AccountListChangeCount++;
            if(AccountList.count() - 1 < SelectedAccount)
                SelectedAccount = AccountList.count() - 1;
            return;
        }
    }
}

void Global::Account::delAccount(int position) {
    if (position < AccountList.count()) {
        AccountList.removeAt(position);
        AccountListChangeCount++;
    }
    if (SelectedAccount >= AccountList.count())
        SelectedAccount = AccountList.count() - 1;
}
int Global::Account::getAccountListChangedCount() {
    return AccountListChangeCount;
}
/**********************************************************************************/
int SelectedAccountChangedCnt = 0;

bool Global::Account::setSelectedAccount(int accountNr) {
    if (accountNr < AccountList.count()) {
        SelectedAccount = accountNr;
        SelectedAccountChangedCnt++;
        return true;
    }
    return false;
}

bool Global::Account::setSelectedAccount(QString accountName) {
    if(!accountName.count())
        return false;
    Global::Account::account_t acc;
    int accCnt = 0;
    foreach(acc, AccountList) {
        if(!acc.accountName.compare(accountName)) {
            if(SelectedAccount != accCnt) {
                SelectedAccount = accCnt;
                SelectedAccountChangedCnt++;
                return true;
            }
            return false;
        }
        accCnt++;
    }
    return false;
}

int Global::Account::getSelectedAccount() {
    return SelectedAccount;
}

QString Global::Account::getSelectedAccountName() {
    if(AccountList.count() == 0)
        SelectedAccount = -1;
    if (SelectedAccount == -1)
        return "";
    return AccountList[SelectedAccount].accountName;
}

int Global::Account::getSelectedAccountChangedCnt() {
    return SelectedAccountChangedCnt;
}
/**********************************************************************************/
QList<QPair<QString,QString>> TickerIconList = {
    QPair<QString,QString>("LYR",":/res/ic/res/ic/ic_lyra_token_foreground.png"),
    QPair<QString,QString>("$TLYR",":/res/ic/res/ic/ic_lyra_token_foreground.png"),
    QPair<QString,QString>("MINT",":/res/ic/res/ic/ic_minted_token_foreground.png"),
    QPair<QString,QString>("$USDT",":/res/ic/res/ic/ic_usdt_foreground.png"),
    QPair<QString,QString>("$TRX",":/res/ic/res/ic/ic_tron_foreground.png"),
    QPair<QString,QString>("$ETH",":/res/ic/res/ic/ic_eth_foreground.png")
};
QString Global::TickerIcon::get(QString ticker) {
    QPair<QString,QString> p;
    foreach(p, TickerIconList) {
        if(!p.first.compare(ticker))
            return p.second;
    }
    return ":/res/ic/res/ic/ic_unknown_foreground.png";
}
/**********************************************************************************/
int TickerPriceListChangedCnt = 0;
QList<QPair<int, QPair<QString, double>>> TickerPriceList;

void Global::TickerPrice::set(QPair<QString, double> tickerPrice) {
    QPair<int, QPair<QString, double>> pair;
    pair.second = tickerPrice;
    for(int cnt = 0; cnt < TickerPriceList.count(); cnt++) {
        if(!TickerPriceList.at(cnt).second.first.compare(tickerPrice.first)) {
            if(pair.second.second == TickerPriceList.at(cnt).second.second)
                return;
            pair.first = TickerPriceList.at(cnt).first + 1;
            TickerPriceList.replace(cnt, pair);
            TickerPriceListChangedCnt++;
            return;
        }
    }
    pair.first = 0;
    TickerPriceList.append(pair);
    TickerPriceListChangedCnt++;
}

double Global::TickerPrice::get(QString ticker) {
    QString t = ticker.remove("$");
    for(int cnt = 0; cnt < TickerPriceList.count(); cnt++) {
        if(!TickerPriceList.at(cnt).second.first.compare(t)) {
            return TickerPriceList.at(cnt).second.second;
        }
    }
    return 0.0;
}

int Global::TickerPrice::getModifyCount(QString ticker) {
    QString t = ticker.remove("$");
    for(int cnt = 0; cnt < TickerPriceList.count(); cnt++) {
        if(!TickerPriceList.at(cnt).second.first.compare(t)) {
            return TickerPriceList.at(cnt).first;
        }
    }
    return 0.0;
}

int Global::TickerPrice::getModifyCount() {
    return TickerPriceListChangedCnt;
}

QList<QPair<int, QPair<QString, double>>> Global::TickerPrice::getList() {
    return TickerPriceList;
}
/**********************************************************************************/
QList<QPair<networkName_e, QList<QString>>> swapTokensAvailableList = QList<QPair<networkName_e, QList<QString>>>(
       {{QPair<networkName_e, QList<QString>>(TESTNET, {"LYR", "Tester/Coin", "$ETH", "$LTT", "$TLYR", "$TRX", "$USDT", "STOKEN/STOKEN"})},
        {QPair<networkName_e, QList<QString>>(MAINNET, {"LYR", "$ETH", "$TRX", "$USDT"})},
        });
QList<QString> Global::Swap::getAvailableTickers() {
    QPair<networkName_e, QList<QString>> list;
    foreach(list, swapTokensAvailableList) {
        if(Net == list.first) {
            return list.second;
        }
    }
    return QList<QString>();
}
/**********************************************************************************/
QList<QPair<QString, QString>> tickerToTokenNameList = {
    QPair<QString, QString>("LYR", "Lyra"),
    QPair<QString, QString>("TLYR", "Lyra"),
    QPair<QString, QString>("BTC", "Bitcoin"),
    QPair<QString, QString>("ETH", "Ethereum"),
    QPair<QString, QString>("ETC", "ethereum-classic"),
    QPair<QString, QString>("TRX", "Tron"),
    QPair<QString, QString>("USDT", "Tether"),
    QPair<QString, QString>("USDC", "usd-coin"),
    QPair<QString, QString>("BUSD", "binance-usd"),
};
QString Global::Util::tickerToTokenName(QString ticker) {
    QString t = ticker.remove('$');
    QPair<QString, QString> p;
    foreach(p, tickerToTokenNameList) {
        if(!p.first.compare(t))
            return p.second;
    }
    return "";
}

QString Global::Util::tokenNameToTicker(QString tokenName) {
    QString t = tokenName.toLower();
    QPair<QString, QString> p;
    foreach(p, tickerToTokenNameList) {
        QString pp = p.second.toLower();
        if(!pp.compare(t, Qt::CaseSensitive))
            return p.first;
    }
    return "";
}
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/

