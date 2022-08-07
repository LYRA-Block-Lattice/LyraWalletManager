#ifndef STORAGEINTERNAL_H
#define STORAGEINTERNAL_H

#include "api/rpc/rpcclass.h"

class StorageInternal
{
public:
    typedef enum {
        OK = 0,
        ALREADY_EXISTS,
        FILE_NOT_EXISTS,
        INVALID_NAME,
        CANNOT_SAVE,
        CANNOT_READ,
        INVALID_FILE,
        FILE_PROTECTED,
        ACCOUNT_NOT_FOUND,
        INVALID_PASSWORD,
        INVALID_PRIVATE_KEY
    }storageError_e;
    static void makeWalletFolder();
    static storageError_e walletCreate(QString password);
    static storageError_e walletDelete(QString password);
    static storageError_e walletRead(QString password);
    static storageError_e accountCreate(QString accountName, QString password, QString pK = nullptr);
    static storageError_e accountDelete(QString accountName, QString password);
};

#endif // STORAGEINTERNAL_H
