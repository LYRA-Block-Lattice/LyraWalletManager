#include "externaladdressvalidator.h"

#include <QDebug>
#include <QCryptographicHash>

#include "crypto/signatures.h"

bool ExternalAddressValidator::tron(QString addr) {
    if(addr.length() < 1)
        return false;
    if (addr.at(0) != TRON_ADDRESSPREFIX)
        return false;
    QByteArray res = signatures::decodeWithCheckSum(addr);
    return res.length() != 0;
}

bool ExternalAddressValidator::ethereum(QString addr) {
    if(addr.length() < 2 || addr.at(0) != '0' || addr.at(1) != 'x')
        return false;
    QString Addr = addr.mid(2);
    QCryptographicHash sha256 (QCryptographicHash::Keccak_256);
    sha256.reset();
    sha256.addData(Addr.toUtf8().toLower());
    QByteArray keccak = sha256.result();
    QString checkChars = keccak.mid(2).data();
    QByteArray cs = Addr.toLower().toUtf8();
     for (int i = 0; i < 40; i++ ) {
        QString c(cs[i]);
        QString tmp = checkChars[i];
        bool ok;
        if ((tmp.toInt(&ok, 16) > 7 && c.toUpper() != c) ||
            (tmp.toInt(&ok, 16) <= 7 && c.toLower() != c)) {
            return false;
        }
    }
    return true;
}
