#include <ui/views/disassembly/code/delegate.hpp>
#include <ui/views/disassembly/code/model.hpp>

#include <QPainter>
#include <QRegularExpression>

namespace ui::views {
    CodeDelegate::CodeDelegate(QObject* parent) : QStyledItemDelegate(parent) {}

    void CodeDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
        painter->save();

        if (option.state & QStyle::State_Selected) {
            QColor highlight(120, 180, 255); /* selection colour */
            highlight.setAlpha(70);
            painter->fillRect(option.rect, highlight);
        }

        QString text = index.data(Qt::DisplayRole).toString();
        QRect rect = option.rect.adjusted(6, 0, -6, 0);

        switch (index.column()) {
            case CodeModel::Address:
                painter->setPen(QColor(140, 140, 140));
                break;
            case CodeModel::Bytes:
                painter->setPen(QColor(160, 160, 170));
                break;
            case CodeModel::Text: {
                static const QRegularExpression mnemonic_re("^(\\S+)");
                auto match = mnemonic_re.match(text);
                if (match.hasMatch()) {
                    QString mnemonic = match.captured(1);
                    QString rest = text.mid(mnemonic.length());

                    painter->setPen(QColor(120, 180, 255));
                    painter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, mnemonic);

                    int mnemonic_width = option.fontMetrics.horizontalAdvance(mnemonic);
                    QRect rest_rect = rect.adjusted(mnemonic_width, 0, 0, 0);

                    painter->setPen(QColor(220, 220, 220));
                    painter->drawText(rest_rect, Qt::AlignVCenter | Qt::AlignLeft, rest);

                    painter->restore();
                    return;
                }
                painter->setPen(QColor(220, 220, 220));
                break;
            }
        }

        painter->drawText(rect, Qt::AlignVCenter | Qt::AlignLeft, text);
        painter->restore();
    }
}
