#ifndef BASE58_H
#define BASE58_H

#include <QByteArray>
#include <QString>


class base58Encoding {
public:
    static QString encode(QByteArray data);
    static QByteArray decode(QString str);
};

#endif // BASE58_H
