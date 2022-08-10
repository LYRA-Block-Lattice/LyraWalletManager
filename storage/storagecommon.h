#ifndef STORAGECOMMON_H
#define STORAGECOMMON_H


class StorageCommon
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
    static void makeSettingsFolder();
};

#endif // STORAGECOMMON_H
