
#include "pagehistoryitemdelegate.h"
#include "pagehistoryitemdelegateprivate.h"
#include <QPainter>

#include "global.h"

PageHistoryItemDelegate::PageHistoryItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_ptr(new PageHistoryItemDelegatePrivate) {

}

PageHistoryItemDelegate::~PageHistoryItemDelegate() {
    delete m_ptr;
}

QSize PageHistoryItemDelegate::iconSize() const {
    return m_ptr->iconSize;
}

void PageHistoryItemDelegate::setIconSize(int width, int height) {
    m_ptr->iconSize = QSize(width, height);
}

QMargins PageHistoryItemDelegate::contentsMargins() const {
    return m_ptr->margins;
}

void PageHistoryItemDelegate::setContentsMargins(int left, int top, int right, int bottom) {
    m_ptr->margins = QMargins(left, top, right, bottom);
}

int PageHistoryItemDelegate::horizontalSpacing() const {
    return m_ptr->spacingHorizontal;
}

void PageHistoryItemDelegate::setHorizontalSpacing(int spacing) {
    m_ptr->spacingHorizontal = spacing;
}

int PageHistoryItemDelegate::verticalSpacing() const {
    return m_ptr->spacingVertical;
}

void PageHistoryItemDelegate::setVerticalSpacing(int spacing) {
    m_ptr->spacingVertical = spacing;
}

void PageHistoryItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                     const QModelIndex &index) const {
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
    QRect tickerRect(m_ptr->tickerBox(opt));

    tickerRect.moveTo(m_ptr->margins.left() + m_ptr->iconSize.width()
                         + m_ptr->spacingHorizontal, contentRect.top());

    f.setPointSizeF(PageHistoryItemDelegatePrivate::timestampFontPointSize(option.font));
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
    QString txt = values.at(0);
    QRect amountRect(m_ptr->amountBox(opt, txt));

    amountRect.moveTo(m_ptr->margins.left() + m_ptr->iconSize.width()
                      + m_ptr->spacingHorizontal, tickerRect.bottom() + verticalSpacing());

    f.setPointSizeF(PageHistoryItemDelegatePrivate::timestampFontPointSize(option.font));
    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(amountRect, Qt::TextSingleLine, txt);

    // Draw value text
    txt = values.at(1);
    QRect valueRect(m_ptr->valueBox(opt, txt));
    valueRect.moveTo(contentRect.right() - valueRect.width(), contentRect.top());
    f.setPointSizeF(PageHistoryItemDelegatePrivate::timestampFontPointSize(option.font));
    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(valueRect, Qt::TextSingleLine,txt);

    // Draw amount text
    txt = values.at(2);
    QRect tokenValueRect(m_ptr->tokenValueBox(opt, txt));

    tokenValueRect.moveTo(contentRect.right() - tokenValueRect.width(), valueRect.bottom() + verticalSpacing());
    f.setPointSizeF(PageHistoryItemDelegatePrivate::timestampFontPointSize(option.font));
    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(tokenValueRect, Qt::TextSingleLine, txt);

    // Draw index
    txt = values.at(4);
    QRect indexRect(m_ptr->indexBox(opt, txt));

    indexRect.moveTo(((rect.width()) / 2) - (indexRect.width() / 2), bottomEdge - indexRect.height());

    f.setPointSizeF(PageHistoryItemDelegatePrivate::indexFontPointSize(option.font));
    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(indexRect, Qt::TextSingleLine, "TX: " + txt);

    painter->restore();
}

QSize PageHistoryItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int textHeight = m_ptr->tickerBox(opt).height()
            + m_ptr->spacingVertical + m_ptr->messageBox(opt).height();
    int iconHeight = m_ptr->iconSize.height();
    int h = textHeight > iconHeight ? textHeight : iconHeight;

    return QSize(opt.rect.width(), m_ptr->margins.top() + h
                 + m_ptr->margins.bottom());
}

PageHistoryItemDelegatePrivate::PageHistoryItemDelegatePrivate() :
    iconSize(16, 16),
    margins(0, 0, 0, 0),
    spacingHorizontal(0),
    spacingVertical(0) {

}

QRect PageHistoryItemDelegatePrivate::tickerBox(const QStyleOptionViewItem &option) const {
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(option.text)
            .adjusted(0, 0, 1, 1);
}

QRect PageHistoryItemDelegatePrivate::nameBox(const QStyleOptionViewItem &option,
                             QString str) const
{
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(str)
            .adjusted(0, 0, 1, 1);
}

QRect PageHistoryItemDelegatePrivate::amountBox(const QStyleOptionViewItem &option, QString str) const {
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(str)
            .adjusted(0, 0, 1, 1);
}

QRect PageHistoryItemDelegatePrivate::valueBox(const QStyleOptionViewItem &option, QString str) const {
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(str)
            .adjusted(0, 0, 1, 1);
}

QRect PageHistoryItemDelegatePrivate::tokenValueBox(const QStyleOptionViewItem &option, QString str) const {
    QFont f(option.font);

    f.setPointSizeF(timestampFontPointSize(option.font));

    return QFontMetrics(f).boundingRect(str)
            .adjusted(0, 0, 1, 1);
}

QRect PageHistoryItemDelegatePrivate::indexBox(const QStyleOptionViewItem &option, QString str) const {
    QFont f(option.font);

    f.setPointSizeF(indexFontPointSize(option.font));

    return QFontMetrics(f).boundingRect("TX: " + str)
            .adjusted(0, 0, 1, 1);
}

qreal PageHistoryItemDelegatePrivate::timestampFontPointSize(const QFont &f) {
    return f.pointSize() * (1/Global::Layout::getXScale());
}

qreal PageHistoryItemDelegatePrivate::indexFontPointSize(const QFont &f) {
    return 0.75*f.pointSize() * (1/Global::Layout::getXScale());
}

QRect PageHistoryItemDelegatePrivate::messageBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}
