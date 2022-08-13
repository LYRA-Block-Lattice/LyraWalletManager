#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QString>

QString Tr(QString s);

class Translate
{
public:
    static QString tr(QString s);
    static void setLanguage(QString language);
};

#endif // TRANSLATE_H
