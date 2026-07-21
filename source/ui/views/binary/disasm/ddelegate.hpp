#pragma once
#include <QStyledItemDelegate>

namespace ui::views {
    class DisassemblyDelegate : public QStyledItemDelegate {
    public:
        explicit DisassemblyDelegate(QObject* parent = nullptr);

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    };
}
