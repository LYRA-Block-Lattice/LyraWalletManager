#include "pagestakinglist.h"
#include "pagestakingitemdelegate.h"
#include <QStandardItemModel>
#include <QDateTime>
#include <QScrollerProperties>
#include <QScroller>

#include "global.h"
#include "color.h"
#include "style.h"

PageStakingList::PageStakingList(QWidget *parent) :
    QListView(parent)
{
    PageStakingItemDelegate *delegate = new PageStakingItemDelegate(this);
    setStyle();
    setModel(new QStandardItemModel(this));
    setItemDelegate(delegate);

    setScale();

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setResizeMode(ResizeMode::Adjust);
    setUpdatesEnabled(true);

//#if defined(__APPLE__) || defined(Q_OS_ANDROID)
//#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || defined(Q_OS_ANDROID)
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    sp.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    sp.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.2);
    sp.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.2);
    sp.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity, 0);
    sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
    sp.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
    sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
    QScroller* scroller = QScroller::scroller(this);
    scroller->grabGesture(this, QScroller::LeftMouseButtonGesture);
    scroller->setScrollerProperties(sp);
//#endif
//#endif
    connect(this, &QAbstractItemView::clicked, this, [=](const QModelIndex index) {
        emit itemClicked(index.row());
    });
}

void PageStakingList::setScale() {
    PageStakingItemDelegate *delegate = (PageStakingItemDelegate *)this->itemDelegate();
    delegate->setContentsMargins(Global::Layout::scaleValue(8), Global::Layout::scaleValue(8), Global::Layout::scaleValue(8), Global::Layout::scaleValue(8));
    delegate->setIconSize(Global::Layout::scaleValue(20), Global::Layout::scaleValue(20));
    delegate->setHorizontalSpacing(Global::Layout::scaleValue(8));
    delegate->setVerticalSpacing(Global::Layout::scaleValue(8));
    //setFont(Global::scaleFont(this->font(), 0.2));
    setFont(QFont("MSShellDlg 2", Global::Layout::scaleValue(12)));
    setFont(Global::Layout::scaleFontOffset(font()));
}

void PageStakingList::setStyle() {
    QPalette p(palette());
    p.setBrush(QPalette::WindowText, QColor(Color::getText2()));
    //p.setBrush(QPalette::Base, QColor("#F0F1F2"));
    p.setBrush(QPalette::Light, QColor(Color::getBackground()));
    //p.setBrush(QPalette::Midlight, QColor("#D3D6D8"));
    p.setBrush(QPalette::Mid, QColor(Color::getText2()));
    p.setBrush(QPalette::Dark, QColor(Color::getText1()));
    p.setBrush(QPalette::Text, QColor(Color::getText2()));
    p.setBrush(QPalette::HighlightedText, QColor(Color::getAdditional()));
    p.setBrush(QPalette::Highlight, QColor(Color::getBackgroundAcc()));
    setPalette(p);
}

void PageStakingList::addMessage(PageStakingList::item_t arg)
{
    QStandardItem *item = new QStandardItem(arg.accountName);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(QString::asprintf("%s\r%s\r%s\r%s\r%s\r%s",
                                    arg.stakingAccountId.toUtf8().data(),
                                    arg.profitingAccountId.toUtf8().data(),
                                    arg.expiryDate.toUtf8().data(),
                                    arg.accountValidity.toUtf8().data(),
                                    arg.amount.toUtf8().data(),
                                    arg.status ? "Valid" : "Expired"),
                  Qt::UserRole);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
    scrollToBottom();
}

void PageStakingList::setTokenValue(QString ticker, double tokenValue) {
    QString stokenValue = Global::Util::normaliseNumber(tokenValue);
    for(int i = 0; i < model()->rowCount(); i++) {
        QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(i, 0));
        QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
        if(!ticker.compare(item->text())) {
            if(values[2].remove(',').toDouble() != tokenValue) {
                QString svalue = Global::Util::normaliseNumber(values[0].remove(',').toDouble() * tokenValue);
                item->setData(QString::asprintf("%s\r%s USD\r%s USD\r%s",
                                                values.at(0).toUtf8().data(),
                                                svalue.toUtf8().data(),
                                                stokenValue.toUtf8().data(),
                                                values.at(3).toUtf8().data()),
                              Qt::UserRole);
            }
        }
    }
}

QString PageStakingList::getName(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    return item->text();
}

QString PageStakingList::getStkId(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    return values[0];
}

QString PageStakingList::getVoting(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    return values[1];
}

QList<QString> PageStakingList::getAccounts() {
    QList<QString> list;
    for(int i = 0; i < model()->rowCount(); i++) {
        QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(i, 0));
        list.append(item->text());
    }
    return list;
}

void PageStakingList::clearAll() {
    static_cast<QStandardItemModel *>(model())->clear();
}


