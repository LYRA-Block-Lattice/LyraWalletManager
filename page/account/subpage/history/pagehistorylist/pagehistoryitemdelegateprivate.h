#ifndef PAGEHISTORYITEMDELEGATEPRIVATE_H
#define PAGEHISTORYITEMDELEGATEPRIVATE_H

#include <qglobal.h>
#include <QRect>
#include <QStyleOptionViewItem>

class PageHistoryItemDelegatePrivate {
    PageHistoryItemDelegatePrivate();

    inline QRect tickerBox(const QStyleOptionViewItem &option) const;
    inline QRect nameBox(const QStyleOptionViewItem &option,
                                 QString str) const;
    inline QRect amountBox(const QStyleOptionViewItem &option, QString str) const;
    inline QRect valueBox(const QStyleOptionViewItem &option, QString str) const;
    inline QRect tokenValueBox(const QStyleOptionViewItem &option, QString str) const;
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
