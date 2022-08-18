#ifndef PAGESTAKINGITEMDELEGATEPRIVATE_H
#define PAGESTAKINGITEMDELEGATEPRIVATE_H

#include <qglobal.h>
#include <QRect>
#include <QStyleOptionViewItem>

class PageStakingItemDelegatePrivate {
    PageStakingItemDelegatePrivate();

    inline QRect allBox(const QStyleOptionViewItem &option, QString text = "") const;
    static inline qreal allFontPointSize(const QFont &f);
    inline QRect messageBox(const QStyleOptionViewItem &option) const;

    QSize iconSize;
    QMargins margins;
    int spacingHorizontal;
    int spacingVertical;

    friend class PageStakingItemDelegate;
};

#endif // PAGEACCOUNTITEMDELEGATEPRIVATE_H
