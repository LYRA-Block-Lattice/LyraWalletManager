#ifndef PAGEHISTORYITEMDELEGATE_H
#define PAGEHISTORYITEMDELEGATE_H

#include <QStyledItemDelegate>

class PageHistoryItemDelegatePrivate;

class PageHistoryItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit PageHistoryItemDelegate(QObject *parent = nullptr);
    ~PageHistoryItemDelegate();

    QSize iconSize() const;
    void setIconSize(int width, int height);

    QMargins contentsMargins() const;
    void setContentsMargins(int left, int top, int right, int bottom);

    int horizontalSpacing() const;
    void setHorizontalSpacing(int spacing);

    int verticalSpacing() const;
    void setVerticalSpacing(int spacing);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

private:
    PageHistoryItemDelegatePrivate *m_ptr;
};

#endif // PAGEACCOUNTITEMDELEGATE_H
