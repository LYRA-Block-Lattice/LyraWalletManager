#ifndef GLOBAL_H
#define GLOBAL_H

#include <QTabWidget>
#include <QWidget>
#include <QRect>
#include <QFont>
#include <QSize>
#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QStandardPaths>

#include "page/accountManagement/pageimportwallet.h"
#include "page/accountManagement/pagenewaccount.h"
#include "page/accountManagement/pagenewwallet.h"
#include "page/accountManagement/pageopenwallet.h"
#include "page/accountManagement/pagerecoveraccount.h"

#include "page/account/subpage/history/pagehistory.h"
#include "page/account/subpage/history/pagetransactiondetail.h"
#include "page/account/subpage/send/pagesend.h"
#include "page/account/subpage/receive/pagereceive.h"

#include "page/more/subpage/settings/pagesettings.h"

#include "globallyra.h"
#include "crypto/base58Encoding.h"
#include "crypto/signatures.h"

#include "storage/storagecommon.h"

#define PAGE_WIDTH      310
#define MENU_BAR_HEIGHT 30

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define FONT_OFFSET     0.75
#elif __linux__
#define FONT_OFFSET     0.75
#else
#define FONT_OFFSET     0.8
#endif

extern QTabWidget *mainTabWidget;

extern PageImportWallet *pageImportWallet;
extern PageNewAccount *pageNewAccount;
extern PageNewWallet *pageNewWallet;
extern PageOpenWallet *pageOpenWallet;
extern PageRecoverAccount *pageRecoverAccount;

extern PageHistory *pageHistory;
extern PageTransactionDetail *pageTransactionDetail;
extern PageSend *pageSend;
extern PageReceive *pageReceive;

extern PageSettings *pageSettings;

extern double yScale;
extern double xScale;
extern int headerHeight;

extern networkName_e Net;
extern QList<QList<QPair<QString,QString>>> NodeList;

#define COMPOSE_WALLET_PARH     QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
#define COMPOSE_WALLET_NAME     COMPOSE_WALLET_PARH + QDir::separator() + Global::Wallet::Name::get() + ".lyr"
#define COMPOSE_SETTINGS_PARH     QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
#define COMPOSE_SETTINGS_NAME     COMPOSE_SETTINGS_PARH + QDir::separator() + Global::Wallet::Name::get() + "_settings.cfg"

namespace Global {
    class Page{
    public:
        typedef enum {
            NONE = -1,
            STAKING = 0,
            SWAP,
            ACCOUNT,
            DEX,
            MORE,

            IMPORT_WALLET = 10,
            NEW_ACCOUNT,
            NEW_WALLET,
            OPEN_WALLET,
            RECOVER_ACCOUNT,

            HISTORY = 20,
            TRANSACTION_DETAIL,
            SEND,
            RECEIVE,

            SETTINGS = 30,
        }PAGE;

        //static void backManagerPage();
        static void goManagerPage(PAGE page, void *arg1 = nullptr);
    };

    class Layout {
    public:
        static QRect getLayoutGeometry() { return QRect(0, headerHeight, PAGE_WIDTH, 532); }
        static QRect getLayoutGeometryScaled() { return QRect(0, headerHeight, (int)((double)PAGE_WIDTH * xScale),
                                                              (int)((double)getLayoutGeometry().height() * yScale)); }

        static QRect getTabLayoutGeometry() { return QRect(0, 0, PAGE_WIDTH, getLayoutGeometry().height() - MENU_BAR_HEIGHT); }
        static QRect getTabLayoutGeometryScaled() { return QRect(0, 0, (int)((double)PAGE_WIDTH * xScale),
                                                                 (int)((double)getTabLayoutGeometry().height() * yScale)); }

        static void setXScale(double sc) { xScale = sc; }
        static void setYScale(double sc) { yScale = sc; }

        static double getXScale() { return xScale; }
        static double getYScale() { return yScale; }

        static void setHeaderHeight(int height) { headerHeight = height; }
        static int getHeaderHeight() { return headerHeight; }

        static int scaleValue(double value) { return (int)((double)value * xScale); }
        static int scaleValue(double value, double scale) { return (int)((double)value * scale); }
        static int scaleValueY(double value) { return (int)((double)value * yScale); }
        static int scaleValueX(double value) { return scaleValue(value); }

        static QFont scaleFont(QFont font) { font.setPointSize((int)((double)font.pointSize() * xScale)); return font; }
        static QFont scaleFont(QFont font, double scale) { font.setPointSize((int)((double)font.pointSize() * (xScale / scale))); return font; }
        static QFont scaleFontOffset(QFont font) { font.setPointSize((int)((double)font.pointSize() * xScale * FONT_OFFSET)); return font; }
        static QFont scaleFontOffset(QFont font, double scale) { font.setPointSize((int)((double)font.pointSize() * xScale * scale)); return font; }

        static QRect scaleRect(QRect rect) { return QRect((int)((double)rect.x() * xScale), (int)((double)rect.y() * xScale),
                                                          (int)((double)rect.width() * xScale), (int)((double)rect.height() * xScale)); }
        static QRect scaleRect(QRect rect, double scale) { return QRect((int)((double)rect.x() * scale), (int)((double)rect.y() * scale),
                                                                        (int)((double)rect.width() * scale), (int)((double)rect.height() * scale)); }
        static QRect scaleRectXY(QRect rect) { return QRect((int)((double)rect.x() * xScale), (int)((double)rect.y() * yScale),
                                                            (int)((double)rect.width() * xScale), (int)((double)rect.height() * yScale)); }
        static QRect scaleRectXYNoDisplace(QRect rect) { return QRect((int)((double)rect.x() * xScale), (int)((double)rect.y() * xScale),
                                                                      (int)((double)rect.width() * xScale), (int)((double)rect.height() * xScale * yScale)); }

        static QSize scaleSize(QSize size) { return QSize((int)((double)size.width() * xScale), (int)((double)size.height() * xScale)); }
        static QSize scaleSize(QSize size, double scale) { return QSize((int)((double)size.width() * scale), (int)((double)size.height() * scale)); }

        static double labelFontScale() {
            double scale = 1.0;
#if defined(__APPLE__) || defined(Q_OS_ANDROID)
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined(Q_OS_ANDROID)
            scale = 0.70;
#else
            scale = 0.9;
#endif
#endif
        return scale;
        }
    };
    /**********************************************************************************/
    class Network {
    public:
        static void setNetwork(networkName_e network) { Net = network; }
        static void setNetwork(QString network) {
            if(!network.compare("TESTNET"))
                Net = networkName_e::TESTNET;
            else if(!network.compare("MAINNET"))
                Net = networkName_e::MAINNET;
            else if(!network.compare("DEVNET"))
                Net = networkName_e::DEVNET;
            else
                Net = networkName_e::NONE;
        }
        static networkName_e getNetwork() { return Net; }
        static QString getNetworkName() {
            switch(Net) {
                case TESTNET: return "TESTNET";
                case MAINNET: return "MAINNET";
                case DEVNET: return "DEVNET";
                default: return "TESTNET";
            }
        }
        static QString getNetworkName(networkName_e i) {
            switch(i) {
                case TESTNET: return "TESTNET";
                case MAINNET: return "MAINNET";
                case DEVNET: return "DEVNET";
                default: return "NONE";
            }
        }
        static QString getNebulaAddress() {
            switch(Global::Network::getNetwork()) {
            case networkName_e::TESTNET:
                return "https://nebulatestnet.lyra.live";
            case networkName_e::MAINNET:
                return "https://nebula.lyra.live";
            case networkName_e::DEVNET:
                return "https://nebuladevnet.lyra.live";
            default:
                return "";
            }
        }
        static QPair<QString,QString> getNodeAddress() { return NodeList[Net][4]; }
    };
    /**********************************************************************************/
    namespace Wallet {
        class Name {
        public:
            static void set(QString name);
            static QString get();
        };
        class Password {
        public:
            static void set(QString p);
            static QString get();
            static bool check(QString p);
            static void destroy();
        };
    };
    /**********************************************************************************/
    class Account {
    public:
        typedef struct account_s {
            QString accountName;
            QString privateKey;
            QString publicId;
        }account_t;
        static bool setAccountList(QList<account_t> accountList);
        static void clearAccountList();
        static QList<account_t> getAccountList();
        static account_t *getAccount();
        static QString getAccountName();
        static QString getAccountPrivateKey();
        static QString getAccountPublicId();
        static void addAccount(account_t account);
        static void addAccount(QString accountName, QString privateKey, QString publicId);
        static void delAccount(QString name);
        static void delAccount(int position);
        static int getAccountListChangedCount();
        /**********************************************************************************/
        static bool setSelectedAccount(int accountNr);
        static bool setSelectedAccount(QString accountName);
        static int getSelectedAccount();
        static QString getSelectedAccountName();
        static int getSelectedAccountChangedCnt();
    };
    /**********************************************************************************/
    class TickerIcon {
    public:
        static QString get(QString ticker);
    };
    /**********************************************************************************/
    class TickerPrice {
    public:
        static void set(QPair<QString, double> tickerPrice);
        static double get(QString ticker);
    };
    /**********************************************************************************/
    /**********************************************************************************/
    /**********************************************************************************/
    /**********************************************************************************/
    /**********************************************************************************/
    typedef struct {

    }accountData_t;
    /**********************************************************************************/
    class Util {
    public:
        static QString getAccountNameAndNetwork() {
            if(Global::Account::getAccountList().count() == 0)
                return "";
            return Global::Account::getSelectedAccountName() + "/" + Global::Network::getNetworkName();
        }
        static QString tickerToSign(QString ticker) { return ticker.replace("tether/", "$"); }
        static QString signToTicker(QString ticker) { return ticker.replace("$", "tether/"); }
        static QString truncateIdHash(QString idHash, int len) {
            if(idHash.length() < len)
                return idHash;
            if(len < 4)
                return "";
            return idHash.left((len / 2) - 1) + "..." + idHash.right((len / 2) - 1);
        }
        static QString normaliseNumber(QString nr) {
            if(nr.indexOf('.') != -1) {
                while ( nr.at(nr.length() - 1) == '0' && nr.at(nr.length() - 1) != '.') {
                    nr.remove(nr.length() - 1, 1);
                }
            }
            return nr;
        }
        static QString normaliseNumber(double nr, int decimals = 8) {
            QString expression = "%." + QString::number(decimals) + "f";
            QString snr = QString::asprintf(expression.toUtf8(), nr);
            if(snr.indexOf('.') != -1) {
                while ( snr.at(snr.length() - 1) == '0' && snr.at(snr.length() - 1) != '.' ) {
                    snr.remove(snr.length() - 1, 1);
                }
            }
            if(snr.at(snr.length() - 1) == '.')
                snr.remove(snr.length() - 1, 1);
            int loc = snr.indexOf('.');
            bool negative = snr.contains('-');
            snr = snr.remove('-');
            if(loc != -1) {
                if(loc > 12) {
                    snr.insert(loc - 3, ',');
                    snr.insert(loc - 6, ',');
                    snr.insert(loc - 9, ',');
                    snr.insert(loc - 12, ',');
                } else if(loc > 9) {
                    snr.insert(loc - 3, ',');
                    snr.insert(loc - 6, ',');
                    snr.insert(loc - 9, ',');
                } else if(loc > 6) {
                    snr.insert(loc - 3, ',');
                    snr.insert(loc - 6, ',');
                } else if(loc > 3)
                    snr.insert(loc - 3, ',');
            } else {
                int len = snr.length();
                if(len > 12) {
                    snr.insert(len - 3, ',');
                    snr.insert(len - 6, ',');
                    snr.insert(len - 9, ',');
                    snr.insert(len - 12, ',');
                } else if(len > 9) {
                    snr.insert(len - 3, ',');
                    snr.insert(len - 6, ',');
                    snr.insert(len - 9, ',');
                } else if(len > 6) {
                    snr.insert(len - 3, ',');
                    snr.insert(len - 6, ',');
                } else if(len > 3)
                    snr.insert(len - 3, ',');
            }
            if(negative)
                snr.insert(0, '-');
            return snr;
        }
        static QString tickerToTokenName(QString ticker);
        static QString tokenNameToTicker(QString tokenName);
    };
    class Check {
    public:
        static bool checkWalletFileNameValid(QString name) {
            static const QRegularExpression nameMatcher("^[a-zA-Z0-9_-]*$", QRegularExpression::CaseInsensitiveOption);
            QRegularExpressionMatch match = nameMatcher.match(name);
            if (!match.hasMatch() || name.length() < 1) {
                return false;
            } else {
                return true;
            }
        }
        static bool checkFileNameWithSpacesValid(QString name) {
            static const QRegularExpression nameMatcher("^[a-zA-Z0-9_-]*$", QRegularExpression::CaseInsensitiveOption);
            foreach(const QString &tmp, name.split(" ")) {
                QRegularExpressionMatch match = nameMatcher.match(tmp);
                if (!match.hasMatch() || name.length() < 1) {
                    return false;
                }
            }
            return true;
        }
        static bool privateKey(QString privateKey) {
            if (signatures::decodePrivateKey(privateKey).count() == 32) {
                return true;
            } else {
                return false;
            }
        }

        static bool accountId(QString accountId) {
            if (signatures::decodeAccountId(accountId).count() == 64) {
                return true;
            } else {
                return false;
            }
        }

        static bool password(QString password1, QString password2) {
            return password1.compare(password2) == 0 && password1.length() >= 8 && password2.length() >= 8 ? true: false;
        }

        static bool password(QString password) {
            return password.length() >= 8 ? true: false;
        }
    };
    class Error {
/*      OK = 0,
        ALREADY_EXISTS,
        NOT_EXISTS,
        INVALID_NAME,
        CANNOT_SAVE,
        CANNOT_READ,
        INVALID_FILE,
        FILE_PROTECTED,
        ACCOUNT_NOT_FOUND,
        INVALID_PASSWORD
        */
    public:
        static bool show(QWidget *parent, StorageCommon::storageError_e err) {
            switch(err) {
            case StorageCommon::ALREADY_EXISTS:
                QMessageBox::critical(parent, "ERROR", "A file with this name already exists.");
                break;
            case StorageCommon::FILE_NOT_EXISTS:
                QMessageBox::critical(parent, "ERROR", "The file with this name does not exist.");
                break;
            case StorageCommon::INVALID_NAME:
                QMessageBox::critical(parent, "ERROR", "The name have invalid format.");
                break;
            case StorageCommon::CANNOT_SAVE:
                QMessageBox::critical(parent, "ERROR", "Unable to save the wallet file.");
                break;
            case StorageCommon::CANNOT_READ:
                QMessageBox::critical(parent, "ERROR", "Unable to read the wallet file.");
                break;
            case StorageCommon::INVALID_FILE:
                QMessageBox::critical(parent, "ERROR", "Invalid file format.");
                break;
            case StorageCommon::FILE_PROTECTED:
                QMessageBox::critical(parent, "ERROR", "The file is protected.");
                break;
            case StorageCommon::ACCOUNT_NOT_FOUND:
                QMessageBox::critical(parent, "ERROR", "Account not found.");
                break;
            case StorageCommon::INVALID_PASSWORD:
                QMessageBox::critical(parent, "ERROR", "Invalid password.");
                break;
            case StorageCommon::INVALID_PRIVATE_KEY:
                QMessageBox::critical(parent, "ERROR", "Invalid private key.");
                break;
            default:
                return false;
            }
            return true;
        }
    };
};

#endif // GLOBAL_H
