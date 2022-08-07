#ifndef PAGEACCOUNTITEMDELEGATEPRIVATE_H
#define PAGEACCOUNTITEMDELEGATEPRIVATE_H

#include <qglobal.h>
#include <QRect>
#include <QStyleOptionViewItem>

class PageAccountItemDelegatePrivate {
    PageAccountItemDelegatePrivate();

    inline QRect tickerBox(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    inline QRect nameBox(const QStyleOptionViewItem &option,
                                 QString str) const;
    inline QRect amountBox(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    inline QRect valueBox(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    inline QRect tokenValueBox(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    inline qreal timestampFontPointSize(const QFont &f) const;
    inline QRect messageBox(const QStyleOptionViewItem &option) const;

    QSize iconSize;
    QMargins margins;
    int spacingHorizontal;
    int spacingVertical;

    friend class PageAccountItemDelegate;
};

#endif // PAGEACCOUNTITEMDELEGATEPRIVATE_H
