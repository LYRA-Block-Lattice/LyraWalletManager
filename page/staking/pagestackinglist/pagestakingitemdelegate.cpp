
#include "pagestakingitemdelegate.h"
#include "pagestakingitemdelegateprivate.h"
#include <QPainter>
#include <QPainterPath>
#include <QColor>
#include <QStandardItemModel>
#include "pagestakinglist.h"

#include "global.h"

static int currentIndex = -1;

PageStakingItemDelegate::PageStakingItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    m_ptr(new PageStakingItemDelegatePrivate)
{

}

PageStakingItemDelegate::~PageStakingItemDelegate()
{
    delete m_ptr;
}

QSize PageStakingItemDelegate::iconSize() const
{
    return m_ptr->iconSize;
}

void PageStakingItemDelegate::setIconSize(int width, int height)
{
    m_ptr->iconSize = QSize(width, height);
}

QMargins PageStakingItemDelegate::contentsMargins() const
{
    return m_ptr->margins;
}

void PageStakingItemDelegate::setContentsMargins(int left, int top, int right, int bottom)
{
    m_ptr->margins = QMargins(left, top, right, bottom);
}

int PageStakingItemDelegate::horizontalSpacing() const
{
    return m_ptr->spacingHorizontal;
}

void PageStakingItemDelegate::setHorizontalSpacing(int spacing)
{
    m_ptr->spacingHorizontal = spacing;
}

int PageStakingItemDelegate::verticalSpacing() const
{
    return m_ptr->spacingVertical;
}

void PageStakingItemDelegate::setVerticalSpacing(int spacing)
{
    m_ptr->spacingVertical = spacing;
}

void PageStakingItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
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
    const int bottomEdge = rect.bottom();
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
    painter->drawLine(lastIndex ? rect.left() : m_ptr->margins.left(),
                      bottomEdge,
                      lastIndex ? rect.right() : contentRect.right(),
                      bottomEdge);

    QList<QString> values = index.data(Qt::UserRole).toString().split("\r");

    //f.setPointSizeF(PageStakingItemDelegatePrivate::allFontPointSize(option.font));
    painter->setFont(f);
    painter->setPen(palette.text().color());
    int pos = 0;
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
    QString stakingId = "Staking account ID";
    QRect stakingIdRect(m_ptr->allBox(opt, stakingId));
    stakingIdRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(stakingIdRect, Qt::TextSingleLine, stakingId);
    // Draw account name value
    QRect stakingIdValueRect(m_ptr->allBox(opt, Global::Util::truncateIdHash(values[0], 17)));
    stakingIdValueRect.moveTo(contentRect.right() - stakingIdValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(stakingIdValueRect, Qt::TextSingleLine, Global::Util::truncateIdHash(values[0], 17));
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString profitingId = "Profiting account ID";
    QRect profitingIdRect(m_ptr->allBox(opt, profitingId));
    profitingIdRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(profitingIdRect, Qt::TextSingleLine, profitingId);
    // Draw account name value
    QRect sprofitingIdValueRect(m_ptr->allBox(opt, Global::Util::truncateIdHash(values[1], 17)));
    sprofitingIdValueRect.moveTo(contentRect.right() - sprofitingIdValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(sprofitingIdValueRect, Qt::TextSingleLine, Global::Util::truncateIdHash(values[1], 17));
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString expiryDate = "Expiry Date";
    QRect expiryDateRect(m_ptr->allBox(opt, expiryDate));
    expiryDateRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(expiryDateRect, Qt::TextSingleLine, expiryDate);
    // Draw account name value
    QRect expiryDateValueRect(m_ptr->allBox(opt, values[2]));
    expiryDateValueRect.moveTo(contentRect.right() - expiryDateValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(expiryDateValueRect, Qt::TextSingleLine, values[2]);
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString accountValidity = "Account validity";
    QRect accountValidityRect(m_ptr->allBox(opt, accountValidity));
    accountValidityRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(accountValidityRect, Qt::TextSingleLine, accountValidity);
    // Draw account name value
    QRect accountValidityValueRect(m_ptr->allBox(opt, values[3]));
    accountValidityValueRect.moveTo(contentRect.right() - accountValidityValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(accountValidityValueRect, Qt::TextSingleLine, values[3]);
    /*************************/
    pos += rowSpace;
    // Draw account name
    QString amount = "Amount";
    QRect amountRect(m_ptr->allBox(opt, amount));
    amountRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(amountRect, Qt::TextSingleLine, amount);
    // Draw account name value
    QRect amountValueRect(m_ptr->allBox(opt, values[4]));
    amountValueRect.moveTo(contentRect.right() - amountValueRect.width() - m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(amountValueRect, Qt::TextSingleLine, values[4]);
    /*************************/
    pos += rowSpace;
    QString status = "Status";
    QRect statusRect(m_ptr->allBox(opt, status));
    statusRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    painter->drawText(statusRect, Qt::TextSingleLine, status);
    // Draw account name value
    QRect statusValueRect(m_ptr->allBox(opt, values[5]));
    statusValueRect.moveTo(contentRect.right() - statusValueRect.width() - m_ptr->iconSize.width() - (m_ptr->spacingVertical * 2),
                           contentRect.top() + pos);
    painter->drawText(statusValueRect, Qt::TextSingleLine, values[5]);
    QPixmap pixmap;
    if(!values[5].compare("Valid"))
        pixmap = QPixmap(":/res/ic/res/ic/dolar.png");
    else
        pixmap = QPixmap(":/res/ic/res/ic/notDolar.png");
    painter->drawPixmap(contentRect.right() - m_ptr->iconSize.width() - m_ptr->spacingVertical,
                        contentRect.top() + pos - ((m_ptr->iconSize.height() - statusValueRect.height()) / 2), m_ptr->iconSize.width(), m_ptr->iconSize.height(), pixmap);


    /*pos += Global::Layout::scaleValue(28);
    QRect stakeButtonRect(0, 0, (contentRect.right() / 2) - (m_ptr->spacingVertical * 2), 32);
    stakeButtonRect.moveTo(m_ptr->margins.left() + m_ptr->spacingVertical, contentRect.top() + pos);
    QPainterPath stakeButtonPath;
    stakeButtonPath.addRoundedRect(stakeButtonRect, 5, 5);
    QPen stakeButtonPen(Qt::lightGray, 1);
    painter->setPen(stakeButtonPen);
    painter->fillPath(stakeButtonPath, QColor(0, 0, 0, 16));
    painter->drawPath(stakeButtonPath);

    painter->setPen(palette.text().color());
    QString stake = "STAKE";
    QRect stakeRect(m_ptr->allBox(opt, stake));
    stakeRect.moveTo(stakeButtonRect.left() + (stakeButtonRect.width() / 2) - (stakeRect.width() / 2),
                       contentRect.top() + pos + 16 - (stakeRect.height() / 2));
    painter->drawText(stakeRect, Qt::TextSingleLine, stake);


    QRect unStakeButtonRect(0, 0, (contentRect.right() / 2) - (m_ptr->spacingVertical * 2), 32);
    unStakeButtonRect.moveTo((contentRect.right() / 2) + (m_ptr->spacingVertical), contentRect.top() + pos);
    QPainterPath unStakeButtonPath;
    unStakeButtonPath.addRoundedRect(unStakeButtonRect, 5, 5);
    QPen unStakeButtonPen(Qt::lightGray, 1);
    painter->setPen(unStakeButtonPen);
    painter->fillPath(unStakeButtonPath, QColor(0, 0, 0, 16));
    painter->drawPath(unStakeButtonPath);

    if(!values[5].compare("Valid"))
        painter->setPen(Qt::lightGray);
    else
        painter->setPen(palette.text().color());
    QString unStake = "UNSTAKE";
    QRect unStakeRect(m_ptr->allBox(opt, unStake));
    unStakeRect.moveTo(unStakeButtonRect.left() + (unStakeButtonRect.width() / 2) - (stakeRect.width() / 2) - m_ptr->spacingVertical,
                       contentRect.top() + pos + 16 - (unStakeRect.height() / 2));
    painter->drawText(unStakeRect, Qt::TextSingleLine, unStake);*/


    painter->restore();

    /*int moi = index.row();
    if(moi != currentIndex) {
        PageStakingList *mymodel = const_cast<PageStakingList*>(dynamic_cast<const PageStakingList*>(index.model()));
        currentIndex = moi;
        mymodel->wantsUpdate(currentIndex);
    }*/
}

QSize PageStakingItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                         const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int rowSpace = Global::Layout::scaleValue(20 * 8);
    //int h = Global::Layout::scaleValue(rowSpace) + m_ptr->spacingVertical;
    //if(currentIndex == index.row())
    //    qDebug() << "Selected: " << currentIndex;
    return QSize(opt.rect.width(), rowSpace);
}

PageStakingItemDelegatePrivate::PageStakingItemDelegatePrivate() :
    iconSize(16, 16),
    margins(0, 0, 0, 0),
    spacingHorizontal(0),
    spacingVertical(0)
{

}

QRect PageStakingItemDelegatePrivate::allBox(const QStyleOptionViewItem &option, QString text) const {
    QFont f(option.font);
    f.setPointSizeF(allFontPointSize(option.font));
    if(text.length() != 0)
        return QFontMetrics(f).boundingRect(text).adjusted(0, 0, 1, 1);
    else
        return QFontMetrics(f).boundingRect(option.text).adjusted(0, 0, 1, 1);
}

qreal PageStakingItemDelegatePrivate::allFontPointSize(const QFont &f)
{
    return f.pointSizeF() * (1/Global::Layout::getXScale());
}

QRect PageStakingItemDelegatePrivate::messageBox(const QStyleOptionViewItem &option) const
{
    return option.fontMetrics.boundingRect(option.text).adjusted(0, 0, 1, 1);
}
