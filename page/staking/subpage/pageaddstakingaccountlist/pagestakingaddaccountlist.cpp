#include "pagestakingaddaccountlist.h"
#include "pagestakingaddaccountitemdelegate.h"
#include <QStandardItemModel>
#include <QDateTime>
#include <QScrollerProperties>
#include <QScroller>

#include "global.h"
#include "color.h"
#include "style.h"

PageStakingAddAccountList::PageStakingAddAccountList(QWidget *parent) :
    QListView(parent)
{
    PageStakingAddAccountItemDelegate *delegate = new PageStakingAddAccountItemDelegate(this);
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

void PageStakingAddAccountList::setScale() {
    PageStakingAddAccountItemDelegate *delegate = (PageStakingAddAccountItemDelegate *)this->itemDelegate();
    delegate->setContentsMargins(Global::Layout::scaleValue(8), Global::Layout::scaleValue(8),
                                 Global::Layout::scaleValue(8), Global::Layout::scaleValue(8));
    delegate->setIconSize(Global::Layout::scaleValue(20), Global::Layout::scaleValue(20));
    delegate->setHorizontalSpacing(Global::Layout::scaleValue(8));
    delegate->setVerticalSpacing(Global::Layout::scaleValue(8));
    //setFont(Global::scaleFont(this->font(), 0.2));
    setFont(QFont("MSShellDlg 2", Global::Layout::scaleValue(12)));
    setFont(Global::Layout::scaleFontOffset(font()));
}

void PageStakingAddAccountList::setStyle() {
    QPalette p(palette());
    p.setBrush(QPalette::WindowText, QColor(Color::getText2()));
    //p.setBrush(QPalette::Base, QColor("#F0F1F2"));
    p.setBrush(QPalette::Light, QColor(Color::getBackground()));
    //p.setBrush(QPalette::Midlight, QColor("#D3D6D8"));
    p.setBrush(QPalette::Mid, QColor(Color::getText2()));
    p.setBrush(QPalette::Dark, QColor(Color::getText1()));
    p.setBrush(QPalette::Text, QColor(Color::getText1()));
    p.setBrush(QPalette::HighlightedText, QColor(Color::getAdditional()));
    p.setBrush(QPalette::Highlight, QColor(Color::getBackgroundAcc()));
    setPalette(p);
}

void PageStakingAddAccountList::addMessage(PageStakingAddAccountList::item_t arg)
{
    QStandardItem *item = new QStandardItem(arg.accountName);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(QString::asprintf("%s\r%s\r%s\r%s\r%s\r%s\r%s%%\r%s\r%s",
                                    arg.accountType.toUtf8().data(),
                                    arg.shareRatio.toUtf8().data(),
                                    arg.seats.toUtf8().data(),
                                    arg.timeStamp.toUtf8().data(),
                                    arg.totalProfit.toUtf8().data(),
                                    arg.totalStaked.toUtf8().data(),
                                    arg.yourShare.toUtf8().data(),
                                    arg.fetch ? "Fetch" : "Not fetch",
                                    arg.pftId.toUtf8().data()),
                  Qt::UserRole);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
    scrollToBottom();
}

void PageStakingAddAccountList::setFetch(int index, double totalStaked, double yourShare) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    item->setData(QString::asprintf("%s\r%s\r%s\r%s\r%s\r%s\r%s%%\r%s\r%s",
                                    values[0].toUtf8().data(),
                                    values[1].toUtf8().data(),
                                    values[2].toUtf8().data(),
                                    values[3].toUtf8().data(),
                                    values[4].toUtf8().data(),
                                    Global::Util::normaliseNumber(totalStaked).toUtf8().data(),
                                    Global::Util::normaliseNumber(yourShare).toUtf8().data(),
                                    "Fetch",
                                    values[8].toUtf8().data()),
                  Qt::UserRole);
}

bool PageStakingAddAccountList::getFetch(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    return values[7].compare("Fetch") ? false : true;
}

QString PageStakingAddAccountList::getPftId(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    return values[8];
}

double PageStakingAddAccountList::getTotalStaked(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    return values[5].remove(',').remove(' ').remove("LYR").toDouble();
}

double PageStakingAddAccountList::getShareRatio(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
    return values[1].remove(',').remove('%').remove(' ').toDouble() / 100.0;
}

QString PageStakingAddAccountList::getName(int index) {
    QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(index, 0));
    return item->text();
}

QList<QString> PageStakingAddAccountList::getAccounts() {
    QList<QString> list;
    for(int i = 0; i < model()->rowCount(); i++) {
        QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(i, 0));
        list.append(item->text());
    }
    return list;
}

void PageStakingAddAccountList::clearAll() {
    static_cast<QStandardItemModel *>(model())->clear();
}


