#pragma once
#include <QStyledItemDelegate>

namespace ui::views {
    class CodeDelegate : public QStyledItemDelegate {
    public:
        explicit CodeDelegate(QObject* parent = nullptr);

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    };
}
