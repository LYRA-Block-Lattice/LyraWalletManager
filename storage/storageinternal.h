#ifndef STORAGEINTERNAL_H
#define STORAGEINTERNAL_H

#include "api/rpc/rpcclass.h"
#include "storagecommon.h"

class StorageInternal
{
public:
    static StorageCommon::storageError_e walletCreate(QString password);
    static StorageCommon::storageError_e walletDelete(QString password);
    static StorageCommon::storageError_e walletRead(QString password);
    static StorageCommon::storageError_e accountCreate(QString accountName, QString password, QString pK = nullptr);
    static StorageCommon::storageError_e accountDelete(QString accountName, QString password);
};

#endif // STORAGEINTERNAL_H
