#ifndef STORAGESETTINGS_H
#define STORAGESETTINGS_H

#include <QString>

class StorageSettings
{
public:
    static void createDefault(bool everwrite = false);
    static void set(QString key, QString value);
    static QString get(QString key, QString defaultValue = "");
};

#endif // STORAGESETTINGS_H
