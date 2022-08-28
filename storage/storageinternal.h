#ifndef STORAGEINTERNAL_H
#define STORAGEINTERNAL_H

#include "api/rpc/rpcclass.h"
#include "storagecommon.h"
#include "errors.h"

class StorageInternal
{
public:
    static void walletBackUp();
    static Global::Errors::Errors_e walletCheck(QString password);
    static Global::Errors::Errors_e walletCreate(QString password);
    static Global::Errors::Errors_e walletDelete(QString password);
    static Global::Errors::Errors_e walletRead(QString password);
    static Global::Errors::Errors_e accountCreate(QString accountName, QString password, QString pK = nullptr);
    static Global::Errors::Errors_e accountDelete(QString accountName, QString password);
    static void walletExport();
    static bool walletImport(QString name);
};

#endif // STORAGEINTERNAL_H
