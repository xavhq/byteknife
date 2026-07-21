#include <ui/views/hex/hviewport.hpp>

#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QContextMenuEvent>
#include <algorithm>

namespace ui::views {
    void HexViewport::SelectAll() {
        if (this->data_.isEmpty())
            return;
        this->selection_start_ = 0;
        this->selection_end_ = this->data_.size() - 1;
        this->viewport()->update();
    }

    void HexViewport::CopySelectionAsHex() {
        QByteArray bytes = this->SelectedBytes();
        if (bytes.isEmpty())
            return;

        QString hex;
        hex.reserve(bytes.size() * 3);

        for (int i = 0; i < bytes.size(); i++) {
            if (i > 0)
                hex += ' ';
            hex += QString("%1").arg(static_cast<quint8>(bytes.at(i)), 2, 16, QChar('0')).toUpper();
        }

        QApplication::clipboard()->setText(hex);
    }

    void HexViewport::CopySelectionAsAscii() {
        QByteArray bytes = this->SelectedBytes();
        if (bytes.isEmpty())
            return;

        QString ascii;
        ascii.reserve(bytes.size());

        for (int i = 0; i < bytes.size(); i++) {
            quint8 b = static_cast<quint8>(bytes.at(i));
            ascii += (b >= 32 && b < 127) ? QChar(static_cast<char>(b)) : QChar('.');
        }

        QApplication::clipboard()->setText(ascii);
    }

    void HexViewport::CopySelectedAddress() {
        if (this->selection_start_ < 0)
            return;

        qint64 offset = std::min(this->selection_start_, this->selection_end_);
        QString addr = QString("0x%1").arg(offset, this->address_width_, 16, QChar('0')).toUpper();
        QApplication::clipboard()->setText(addr);
    }

    void HexViewport::contextMenuEvent(QContextMenuEvent* event) {
        if (this->data_.isEmpty())
            return;

        /* if right-clicking outside the current selection, move selection to that byte first */
        qint64 clicked_offset = this->OffsetFromPoint(event->pos());
        bool has_selection = (
            this->selection_start_ >= 0 &&
            clicked_offset >= std::min(this->selection_start_, this->selection_end_) &&
            clicked_offset <= std::max(this->selection_start_, this->selection_end_)
        );

        if (!has_selection) {
            this->selection_start_ = clicked_offset;
            this->selection_end_ = clicked_offset;
            this->viewport()->update();
        }

        QMenu menu(this);

        QAction* copy_hex = menu.addAction("Copy as Hex");
        connect(copy_hex, &QAction::triggered, this, &HexViewport::CopySelectionAsHex);

        QAction* copy_ascii = menu.addAction("Copy as Text");
        connect(copy_ascii, &QAction::triggered, this, &HexViewport::CopySelectionAsAscii);

        QAction* copy_addr = menu.addAction("Copy Address");
        connect(copy_addr, &QAction::triggered, this, &HexViewport::CopySelectedAddress);

        menu.addSeparator();

        QAction* select_all = menu.addAction("Select All");
        connect(select_all, &QAction::triggered, this, &HexViewport::SelectAll);

        QAction* find_pattern = menu.addAction("Find Pattern...");
        connect(find_pattern, &QAction::triggered, this, &HexViewport::FindPatternFromUser);

        menu.exec(event->globalPos());
    }
}
