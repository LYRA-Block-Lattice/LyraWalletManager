
#include "pageaccountitemdelegate.h"
#include "pageaccountitemdelegateprivate.h"
#include <QPainter>

#include "global.h"

PageAccountItemDelegate::PageAccountItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_ptr(new PageAccountItemDelegatePrivate)
{

}

PageAccountItemDelegate::~PageAccountItemDelegate()
{
    delete m_ptr;
}

QSize PageAccountItemDelegate::iconSize() const
{
    return m_ptr->iconSize;
}

void PageAccountItemDelegate::setIconSize(int width, int height)
{
    m_ptr->iconSize = QSize(width, height);
}

QMargins PageAccountItemDelegate::contentsMargins() const
{
    return m_ptr->margins;
}

void PageAccountItemDelegate::setContentsMargins(int left, int top, int right, int bottom)
{
    m_ptr->margins = QMargins(left, top, right, bottom);
}

int PageAccountItemDelegate::horizontalSpacing() const
{
    return m_ptr->spacingHorizontal;
}

void PageAccountItemDelegate::setHorizontalSpacing(int spacing)
{
    m_ptr->spacingHorizontal = spacing;
}

int PageAccountItemDelegate::verticalSpacing() const
{
    return m_ptr->spacingVertical;
}

void PageAccountItemDelegate::setVerticalSpacing(int spacing)
{
    m_ptr->spacingVertical = spacing;
}

void PageAccountItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                     const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);
    const QRect &contentRect(rect.adjusted(m_ptr->margins.left(),
                                               m_ptr->margins.top(),
                                               -m_ptr->margins.right(),
                                               -m_ptr->margins.bottom()));
    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
    const bool hasIcon = !opt.icon.isNull();
    const int bottomEdge = rect.bottom();
    QFont f(opt.font);

    f.setPointSize(m_ptr->timestampFontPointSize(opt.font));

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    // Draw background
    painter->fillRect(rect, /*opt.state & QStyle::State_Selected ?
                          palette.highlight().color() :*/
                          palette.light().color());

    // Draw bottom line
    painter->setPen(lastIndex ? palette.dark().color()
                              : palette.mid().color());
    painter->setPen(lastIndex ? palette.dark().color()
                              : palette.mid().color());
    painter->drawLine(lastIndex ? rect.left() : m_ptr->margins.left(),
                      bottomEdge,
                      lastIndex ? rect.right() : contentRect.right(),
                      bottomEdge);

    QList<QString> values = index.data(Qt::UserRole).toString().split("\r");
    // Draw item icon
    if (hasIcon)
        painter->drawPixmap(contentRect.left(), contentRect.top(),
                            opt.icon.pixmap(m_ptr->iconSize));

    // Draw ticker text
    QRect tickerRect(m_ptr->tickerBox(opt, index));

    tickerRect.moveTo(m_ptr->margins.left() + m_ptr->iconSize.width()
                         + m_ptr->spacingHorizontal, contentRect.top());

    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(tickerRect, Qt::TextSingleLine,
                      opt.text);

    QRect nameRect(m_ptr->nameBox(opt, values.at(3)));
    nameRect.moveTo(tickerRect.x() + tickerRect.width() + 3, contentRect.top());
    painter->setPen(palette.highlightedText().color());
    painter->drawText(nameRect, Qt::TextSingleLine,
                      values.at(3));
    // Draw amount text
    QRect amountRect(m_ptr->amountBox(opt, index));

    amountRect.moveTo(m_ptr->margins.left() + m_ptr->iconSize.width()
                      + m_ptr->spacingHorizontal, tickerRect.bottom() + verticalSpacing());

    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(amountRect, Qt::TextSingleLine, values.at(0));

    // Draw value text
    QRect valueRect(m_ptr->valueBox(opt, index));

    valueRect.moveTo(contentRect.right() - valueRect.width(), contentRect.top());

    if(values.at(1).length() > 0) {
        painter->setFont(f);
        painter->setPen(palette.text().color());
        painter->drawText(valueRect, Qt::TextSingleLine,values.at(1));
    }

    // Draw amount text
    QRect tokenValueRect(m_ptr->tokenValueBox(opt, index));

    tokenValueRect.moveTo(contentRect.right() - tokenValueRect.width(), valueRect.bottom() + verticalSpacing());

    if(values.at(2).length() > 0) {
        painter->setFont(f);
        painter->setPen(palette.text().color());
        painter->drawText(tokenValueRect, Qt::TextSingleLine, values.at(2));
    }

    painter->restore();
}

QSize PageAccountItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = m_ptr->tickerBox(opt, index).height()
            + m_ptr->spacingVertical + m_ptr->messageBox(opt).height();
    int iconHeight = m_ptr->iconSize.height();
    int h = textHeight > iconHeight ? textHeight : iconHeight;

    return QSize(opt.rect.width(), m_ptr->margins.top() + h
                 + m_ptr->margins.bottom());
}

PageAccountItemDelegatePrivate::PageAccountItemDelegatePrivate() :
    iconSize(16, 16),
    margins(0, 0, 0, 0),
    spacingHorizontal(0),
    spacingVertical(0)
{

}

QRect PageAccountItemDelegatePrivate::tickerBox(const QStyleOptionViewItem &option,
                             const QModelIndex &/*index*/) const
{
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(option.text)
            .adjusted(0, 0, 1, 1);
}

QRect PageAccountItemDelegatePrivate::nameBox(const QStyleOptionViewItem &option,
                             QString str) const
{
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(str)
            .adjusted(0, 0, 1, 1);
}

QRect PageAccountItemDelegatePrivate::amountBox(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(index.data(Qt::UserRole).toString().split("\r").at(0))
            .adjusted(0, 0, 1, 1);
}

QRect PageAccountItemDelegatePrivate::valueBox(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(index.data(Qt::UserRole).toString().split("\r").at(1))
            .adjusted(0, 0, 1, 1);
}

QRect PageAccountItemDelegatePrivate::tokenValueBox(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(index.data(Qt::UserRole).toString().split("\r").at(2))
            .adjusted(0, 0, 1, 1);
}

qreal PageAccountItemDelegatePrivate::timestampFontPointSize(const QFont &f) const
{
    return f.pointSize() * (1/Global::Layout::getXScale());
}

QRect PageAccountItemDelegatePrivate::messageBox(const QStyleOptionViewItem &option) const
{
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}
