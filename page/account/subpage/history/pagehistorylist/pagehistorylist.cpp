#include "pagehistorylist.h"
#include "pagehistoryitemdelegate.h"
#include <QStandardItemModel>
#include <QDateTime>
#include <QScrollerProperties>
#include <QScroller>

#include "global.h"
#include "color.h"

PageHistoryList::PageHistoryList(QWidget *parent) :
    QListView(parent)
{
    PageHistoryItemDelegate *delegate = new PageHistoryItemDelegate(this);
    setStyle();
    setModel(new QStandardItemModel(this));
    setItemDelegate(delegate);

    setScale();

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

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
    connect(this, &QAbstractItemView::clicked,
                this, &PageHistoryList::onListClicked);
}

void PageHistoryList::setScale() {
    PageHistoryItemDelegate *delegate = (PageHistoryItemDelegate *)this->itemDelegate();
    delegate->setContentsMargins(Global::Layout::scaleValue(8), Global::Layout::scaleValue(8), Global::Layout::scaleValue(8), Global::Layout::scaleValue(8));
    delegate->setIconSize(Global::Layout::scaleValue(36), Global::Layout::scaleValue(36));
    delegate->setHorizontalSpacing(Global::Layout::scaleValue(8));
    delegate->setVerticalSpacing(Global::Layout::scaleValue(8));
    //setFont(Global::scaleFont(this->font(), 0.2));
    setFont(QFont("MSShellDlg 2", Global::Layout::scaleValue(14)));
    setFont(Global::Layout::scaleFontOffset(font()));
}

void PageHistoryList::setStyle() {
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

void PageHistoryList::addMessage(const QPixmap &pixmap, const QString &ticker, const double &amount, const double &value, const double &tokenValue, const qint64 &height) {
    QStandardItem *item = new QStandardItem(QIcon(pixmap), ticker);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QString samount = Global::Util::normaliseNumber(amount);
    QString svalue = Global::Util::normaliseNumber(value);
    QString stokenValue = Global::Util::normaliseNumber(tokenValue);
    item->setData(QString::asprintf("%s\r%s USD\r%s USD\r%s\r%d",
                                    samount.toUtf8().data(),
                                    svalue.toUtf8().data(),
                                    stokenValue.toUtf8().data(),
                                    Global::Util::tickerToTokenName(ticker).toUtf8().data(),
                                    (int)height),
                  Qt::UserRole);

    static_cast<QStandardItemModel *>(model())->appendRow(item);
    scrollToBottom();
}

void PageHistoryList::setTokenValue(QString ticker, double tokenValue) {
    QString stokenValue = Global::Util::normaliseNumber(tokenValue);
    for(int i = 0; i < model()->rowCount(); i++) {
        QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(i, 0));
        QList<QString> values = item->data(Qt::UserRole).toString().split("\r");
        if(!ticker.compare(item->text())) {
            if(values[2].remove(',').toDouble() != tokenValue) {
                QString svalue = Global::Util::normaliseNumber(values[0].remove(',').toDouble() * tokenValue);
                item->setData(QString::asprintf("%s\r%s USD\r%s USD\r%s\r%d",
                                                values.at(0).toUtf8().data(),
                                                svalue.toUtf8().data(),
                                                stokenValue.toUtf8().data(),
                                                values.at(3).toUtf8().data(),
                                                values[4].toInt()),
                              Qt::UserRole);
            }
        }
    }
}

QList<QString> PageHistoryList::getTickers() {
    QList<QString> list;
    for(int i = 0; i < model()->rowCount(); i++) {
        QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(model()->index(i, 0));
        list.append(item->text());
    }
    return list;
}

void PageHistoryList::clearAll() {
    static_cast<QStandardItemModel *>(model())->clear();
}

void PageHistoryList::onListClicked(const QModelIndex index) {
    //qDebug() << index.row();
    emit itemClicked(index.row());
}


