
#include "pagestakingaddaccountitemdelegate.h"
#include "pagestakingaddaccountitemdelegateprivate.h"
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QStandardItemModel>
#include "pagestakingaddaccountlist.h"

#include "global.h"

//static int currentIndex = -1;

PageStakingAddAccountItemDelegate::PageStakingAddAccountItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_ptr(new PageStakingAddAccountItemDelegatePrivate)
{

}

PageStakingAddAccountItemDelegate::~PageStakingAddAccountItemDelegate()
{
    delete m_ptr;
}

QSize PageStakingAddAccountItemDelegate::iconSize() const
{
    return m_ptr->iconSize;
}

void PageStakingAddAccountItemDelegate::setIconSize(int width, int height)
{
    m_ptr->iconSize = QSize(width, height);
}

QMargins PageStakingAddAccountItemDelegate::contentsMargins() const
{
    return m_ptr->margins;
}

void PageStakingAddAccountItemDelegate::setContentsMargins(int left, int top, int right, int bottom)
{
    m_ptr->margins = QMargins(left, top, right, bottom);
}

int PageStakingAddAccountItemDelegate::horizontalSpacing() const
{
    return m_ptr->spacingHorizontal;
}

void PageStakingAddAccountItemDelegate::setHorizontalSpacing(int spacing)
{
    m_ptr->spacingHorizontal = spacing;
}

int PageStakingAddAccountItemDelegate::verticalSpacing() const
{
    return m_ptr->spacingVertical;
}

void PageStakingAddAccountItemDelegate::setVerticalSpacing(int spacing)
{
    m_ptr->spacingVertical = spacing;
}

void PageStakingAddAccountItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
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
    //const bool hasIcon = !opt.icon.isNull();
    //const int bottomEdge = rect.bottom();
    QFont f(opt.font);

    f.setPointSizeF(m_ptr->allFontPointSize(opt.font));

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    // Draw background
    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                          palette.highlight().color() :
                          palette.light().color());
    // Draw bottom line
    painter->setPen(lastIndex ? palette.dark().color()
                              : palette.mid().color());
    /*painter->drawLine(lastIndex ? rect.left() : m_ptr->margins.left(),
                      bottomEdge,
                      lastIndex ? rect.right() : contentRect.right(),
                      bottomEdge);*/

    painter->drawRoundedRect(contentRect.x(),
                             contentRect.y(),
                             contentRect.width(),
                             contentRect.height(), 5, 5);

    QList<QString> values = index.data(Qt::UserRole).toString().split("\r");

    //f.setPointSizeF(PageStakingAddAccountItemDelegatePrivate::allFontPointSize(option.font));
    painter->setFont(f);
    painter->setPen(palette.text().color());
    int pos = Global::Layout::scaleValue(10);
    int rowSpace = Global::Layout::scaleValue(20);
    /*************************/
    // Draw account name
    QString name = "Account name";
    QRect accountNameRect(m_ptr->allBox(opt, name));
    accountNameRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(accountNameRect, Qt::TextSingleLine, name);
    // Draw account name value
    QRect accountNameValueRect(m_ptr->allBox(opt, opt.text));
    accountNameValueRect.moveTo(contentRect.right() - accountNameValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(accountNameValueRect, Qt::TextSingleLine, opt.text);
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString accountType = "Account type";
    QRect accountTypeRect(m_ptr->allBox(opt, accountType));
    accountTypeRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(accountTypeRect, Qt::TextSingleLine, accountType);
    // Draw account name value
    QRect accountTypeValueRect(m_ptr->allBox(opt, Global::Util::truncateIdHash(values[0], 17)));
    accountTypeValueRect.moveTo(contentRect.right() - accountTypeValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(accountTypeValueRect, Qt::TextSingleLine, Global::Util::truncateIdHash(values[0], 17));
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString shareRatio = "Share ratio";
    QRect shareRatioRect(m_ptr->allBox(opt, shareRatio));
    shareRatioRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(shareRatioRect, Qt::TextSingleLine, shareRatio);
    // Draw account name value
    QRect shareRatioValueRect(m_ptr->allBox(opt, Global::Util::truncateIdHash(values[1], 17)));
    shareRatioValueRect.moveTo(contentRect.right() - shareRatioValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(shareRatioValueRect, Qt::TextSingleLine, Global::Util::truncateIdHash(values[1], 17));
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString seats = "Seats";
    QRect seatsRect(m_ptr->allBox(opt, seats));
    seatsRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(seatsRect, Qt::TextSingleLine, seats);
    // Draw account name value
    QRect seatsValueRect(m_ptr->allBox(opt, values[2]));
    seatsValueRect.moveTo(contentRect.right() - seatsValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(seatsValueRect, Qt::TextSingleLine, values[2]);
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString timeStamp = "Time stamp";
    QRect timeStampRect(m_ptr->allBox(opt, timeStamp));
    timeStampRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(timeStampRect, Qt::TextSingleLine, timeStamp);
    // Draw account name value
    QRect timeStampValueRect(m_ptr->allBox(opt, values[3]));
    timeStampValueRect.moveTo(contentRect.right() - timeStampValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(timeStampValueRect, Qt::TextSingleLine, values[3]);
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString totalProfit = "Total profit";
    QRect totalProfitRect(m_ptr->allBox(opt, totalProfit));
    totalProfitRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(totalProfitRect, Qt::TextSingleLine, totalProfit);
    // Draw account name value
    QRect totalProfitValueRect(m_ptr->allBox(opt, values[4]));
    totalProfitValueRect.moveTo(contentRect.right() - totalProfitValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(totalProfitValueRect, Qt::TextSingleLine, values[4]);
    /*************************/
    pos += rowSpace;
    QString totalStaked = "Total staked";
    QRect totalStakedRect(m_ptr->allBox(opt, totalStaked));
    totalStakedRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(totalStakedRect, Qt::TextSingleLine, totalStaked);
    // Draw account name value
    QRect totalStakedValueRect(m_ptr->allBox(opt, values[5]));
    totalStakedValueRect.moveTo(contentRect.right() - totalStakedValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(totalStakedValueRect, Qt::TextSingleLine, values[5]);
    /*************************/
    pos += rowSpace;
    QString yourShare = "Your share will be";
    QRect yourShareRect(m_ptr->allBox(opt, yourShare));
    yourShareRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(yourShareRect, Qt::TextSingleLine, yourShare);
    // Draw account name value
    QRect yourShareValueRect(m_ptr->allBox(opt, values[6]));
    yourShareValueRect.moveTo(contentRect.right() - yourShareValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(yourShareValueRect, Qt::TextSingleLine, values[6]);
    /*************************/
    pos += rowSpace;
    QString fetch = values[7];
    QRect fetchRect(m_ptr->allBox(opt, fetch));
    fetchRect.moveTo((contentRect.right() / 2) - (fetchRect.width() / 2) + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(fetchRect, Qt::TextSingleLine, fetch);

    painter->restore();
}

QSize PageStakingAddAccountItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    //int rowSpace = Global::Layout::scaleValue(20 * 7);
    //int h = Global::Layout::scaleValue(rowSpace) + m_ptr->spacingVertical;
    /*int h = Global::Layout::scaleValue(Global::Layout::scaleValue(155))
            + m_ptr->spacingVertical + m_ptr->messageBox(opt).height();*/
    return QSize(opt.rect.width(), opt.rect.height());
}

PageStakingAddAccountItemDelegatePrivate::PageStakingAddAccountItemDelegatePrivate() :
    iconSize(16, 16),
    margins(0, 0, 0, 0),
    spacingHorizontal(0),
    spacingVertical(0) {

}

QRect PageStakingAddAccountItemDelegatePrivate::allBox(const QStyleOptionViewItem &option, QString text) const {
    QFont f(option.font);
    f.setPointSizeF(allFontPointSize(option.font));
    if(text.length() != 0)
        return QFontMetrics(f).boundingRect(text).adjusted(0, 0, 1, 1);
    else
        return QFontMetrics(f).boundingRect(option.text).adjusted(0, 0, 1, 1);
}

qreal PageStakingAddAccountItemDelegatePrivate::allFontPointSize(const QFont &f) {
    return f.pointSizeF() * (1/Global::Layout::getXScale());
}

QRect PageStakingAddAccountItemDelegatePrivate::messageBox(const QStyleOptionViewItem &option) const {
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}
