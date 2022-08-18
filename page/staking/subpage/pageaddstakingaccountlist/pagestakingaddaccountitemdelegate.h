#ifndef PAGESTAKINGADDACCOUNTITEMDELEGATE_H
#define PAGESTAKINGADDACCOUNTITEMDELEGATE_H

#include <QStyledItemDelegate>

class PageStakingAddAccountItemDelegatePrivate;

class PageStakingAddAccountItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit PageStakingAddAccountItemDelegate(QObject *parent = nullptr);
    ~PageStakingAddAccountItemDelegate();

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
    PageStakingAddAccountItemDelegatePrivate *m_ptr;
};

#endif // PAGEACCOUNTITEMDELEGATE_H
