#ifndef PAGEHISTORYITEMDELEGATEPRIVATE_H
#define PAGEHISTORYITEMDELEGATEPRIVATE_H

#include <qglobal.h>
#include <QRect>
#include <QStyleOptionViewItem>

class PageHistoryItemDelegatePrivate {
    PageHistoryItemDelegatePrivate();

    inline QRect allBox(const QStyleOptionViewItem &option, QString text) const;
    static qreal allFontPointSize(const QFont &f);

    inline QRect indexBox(const QStyleOptionViewItem &option, QString str) const;
    static inline qreal timestampFontPointSize(const QFont &f);
    static inline qreal indexFontPointSize(const QFont &f);
    inline QRect messageBox(const QStyleOptionViewItem &option) const;

    QSize iconSize;
    QMargins margins;
    int spacingHorizontal;
    int spacingVertical;

    friend class PageHistoryItemDelegate;
};

#endif // PAGEACCOUNTITEMDELEGATEPRIVATE_H
