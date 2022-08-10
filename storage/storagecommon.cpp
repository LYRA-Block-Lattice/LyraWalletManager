#include "storagecommon.h"

#include <QDir>

#include "global.h"

void StorageCommon::makeWalletFolder() {
    QDir d(COMPOSE_WALLET_PARH);
    if(!d.exists(COMPOSE_WALLET_PARH))
        d.mkdir(COMPOSE_WALLET_PARH);
}

void StorageCommon::makeSettingsFolder() {
    QDir d(COMPOSE_WALLET_PARH);
    if(!d.exists(COMPOSE_WALLET_PARH))
        d.mkdir(COMPOSE_WALLET_PARH);
}

