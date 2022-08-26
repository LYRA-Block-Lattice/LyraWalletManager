#ifndef EXTERNALADDRESSVALIDATOR_H
#define EXTERNALADDRESSVALIDATOR_H

#include <QString>

#define TRON_ADDRESSPREFIX      'T'

class ExternalAddressValidator {
public:
    static bool tron(QString addr);
    static bool ethereum(QString addr);
};

#endif // EXTERNALADDRESSVALIDATOR_H
