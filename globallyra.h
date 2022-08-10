#ifndef GLOBALLYRA_H
#define GLOBALLYRA_H

#include <QString>
#include <QList>

#define LYRA_CHECKSUM_SIZE_IN_BYTES             4
#define ADDRESSPREFIX                           'L'
#define LYRA_MAX_SUPPLY                         10_000_000_000
#define LYRA_TX_FEE                             1.0
#define BREAKING_STAKE_CONTRACT_FEE             "0.8"
#define SYMBOL_FOR_TETHERED_TOKEN               "$"
#define LYRA_RPC_API_URL                        "/api/v1/socket"
#define LYRA_NODE_API_URL                       "/api/Node"
#define LYRA_DEX_API_URL                        "https://dex.lyra.live/api/Dex"
#define LYRA_STAKE_MIN_DAYS                     3
#define LYRA_STAKE_MAX_DAYS                     36500
#define LYRA_DECIMALS                           8

#define RPC_CONNECTION_TIMEOUT_MS               20000

typedef enum {
    NONE = -1,
    TESTNET = 0,
    MAINNET = 1,
    DEVNET = 2
} networkName_e;

static QList<QString> networkNameList = QList<QString>({"TESTNET", "MAINNET"/*, "DEVNET"*/});

/*class GlobalLyra {
    static double getAmount(long long amount) {

    };
};*/

#endif // GLOBALLYRA_H
