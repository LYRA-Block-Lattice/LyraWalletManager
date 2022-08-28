#ifndef ERROR_H
#define ERROR_H

namespace Global {
    class Errors {
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
        }Errors_e;
    };
}

#endif // ERROR_H
