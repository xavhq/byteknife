#include <ui/views/hex/hviewport.hpp>

#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFontMetricsF>
#include <QEvent>
#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QContextMenuEvent>
#include <algorithm>

namespace ui::views {
    HexViewport::HexViewport(QWidget* parent) : QAbstractScrollArea(parent) {
        this->setFocusPolicy(Qt::StrongFocus);
        this->viewport()->setCursor(Qt::IBeamCursor);
        this->UpdateColors();
        this->UpdateMetrics();
    }

    void HexViewport::SetReadOnly(bool read_only) {
        this->read_only_ = read_only;
    }

    void HexViewport::SetAddressWidth(int width) {
        this->address_width_ = width;
        this->UpdateScrollRange();
        this->viewport()->update();
    }

    void HexViewport::UpdateColors() {
        const QPalette& pal = this->palette();

        this->bg_color_ = pal.color(QPalette::Base);
        this->bg_alt_color_ = pal.color(QPalette::AlternateBase);
        this->address_color_ = pal.color(QPalette::Disabled, QPalette::Text);
        this->hex_color_ = pal.color(QPalette::Text);
        this->ascii_color_ = pal.color(QPalette::Text);
        this->separator_color_ = pal.color(QPalette::Mid);
        this->unused_color_ = pal.color(QPalette::Mid);

        QColor selection = pal.color(QPalette::Highlight);
        selection.setAlpha(120);
        this->selection_bg_ = selection;
    }

    void HexViewport::changeEvent(QEvent* event) {
        QAbstractScrollArea::changeEvent(event);

        if (event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange) {
            this->UpdateColors();
            this->viewport()->update();
        }
    }

    void HexViewport::SetData(const QByteArray& data) {
        this->data_ = data;
        this->selection_start_ = -1;
        this->selection_end_ = -1;

        this->UpdateMetrics();
        this->RecalculateBytesPerLine();
        this->UpdateScrollRange();
        this->verticalScrollBar()->setValue(0);
        this->viewport()->update();
    }

    void HexViewport::Clear() {
        this->data_.clear();
        this->selection_start_ = -1;
        this->selection_end_ = -1;
        this->UpdateScrollRange();
        this->viewport()->update();
    }

    QByteArray HexViewport::SelectedBytes() const {
        if (this->selection_start_ < 0 || this->selection_end_ < 0)
            return {};

        qint64 start = std::min(this->selection_start_, this->selection_end_);
        qint64 end = std::max(this->selection_start_, this->selection_end_);
        return this->data_.mid(static_cast<int>(start), static_cast<int>(end - start + 1));
    }

    void HexViewport::UpdateMetrics() {
        QFontMetricsF fm(this->font());
        this->char_width_ = fm.horizontalAdvance(' ');
        this->line_height_ = fm.height() + 4;
    }

    qint64 HexViewport::LineCount() const {
        if (this->bytes_per_line_ <= 0) return 0;
        return (this->data_.size() + this->bytes_per_line_ - 1) / this->bytes_per_line_;
    }

    int HexViewport::VisibleLines() const {
        if (this->line_height_ <= 0) return 1;
        return std::max(1, static_cast<int>(this->viewport()->height() / this->line_height_));
    }

    qreal HexViewport::ContentWidth() const {
        return this->AsciiColumnX() + this->bytes_per_line_ * this->char_width_ + this->char_width_ * 2;
    }

    void HexViewport::RecalculateBytesPerLine() {
        if (!this->auto_fit_)
            return;

        qreal available = this->viewport()->width() - this->HexColumnX() - this->char_width_ * 2;
        if (available <= 0)
            return;

        int best = this->min_bytes_per_line_;

        for (int candidate = this->min_bytes_per_line_; candidate <= this->max_bytes_per_line_; candidate++) {
            qreal group_gaps = (candidate / 8) * this->char_width_;
            qreal width_needed = (candidate * 3 * this->char_width_) + (candidate * this->char_width_) + group_gaps;

            if (width_needed <= available)
                best = candidate;
            else
                break;
        }

        if (best != this->bytes_per_line_) {
            this->bytes_per_line_ = best;
            this->selection_start_ = -1;
            this->selection_end_ = -1;
        }
    }

    void HexViewport::UpdateScrollRange() {
        qint64 lines = this->LineCount();
        int visible = this->VisibleLines();

        this->verticalScrollBar()->setRange(0, static_cast<int>(std::max<qint64>(0, lines - visible)));
        this->verticalScrollBar()->setPageStep(std::max(1, visible - 1));

        qreal content_width = this->ContentWidth();
        int viewport_width = this->viewport()->width();

        if (this->centered_ && content_width < viewport_width) {
            this->content_offset_x_ = (viewport_width - content_width) / 2.0;
            this->horizontalScrollBar()->setRange(0, 0);
        } else {
            this->content_offset_x_ = 0;
            int overflow = static_cast<int>(content_width - viewport_width);
            this->horizontalScrollBar()->setRange(0, std::max(0, overflow));
        }
    }

    qreal HexViewport::HexColumnX() const {
        return (1 + this->address_width_ + 2) * this->char_width_;
    }

    qreal HexViewport::ByteCellX(int column) const {
        qreal x = this->HexColumnX();
        for (int i = 0; i < column; i++) {
            x += this->char_width_ * 3;
            if ((i + 1) % 8 == 0)
                x += this->char_width_;
        }
        return x;
    }

    qreal HexViewport::AsciiColumnX() const {
        qreal x = this->ByteCellX(this->bytes_per_line_ - 1) + this->char_width_ * 3;
        x += this->char_width_ * 2;
        return x;
    }

    bool HexViewport::IsSelected(qint64 offset) const {
        if (this->selection_start_ < 0 || this->selection_end_ < 0)
            return false;

        qint64 start = std::min(this->selection_start_, this->selection_end_);
        qint64 end = std::max(this->selection_start_, this->selection_end_);
        return offset >= start && offset <= end;
    }

    void HexViewport::paintEvent(QPaintEvent* event) {
        Q_UNUSED(event);

        QPainter painter(this->viewport());
        painter.setFont(this->font());
        painter.fillRect(this->viewport()->rect(), this->bg_color_);

        qint64 first_line = this->verticalScrollBar()->value();
        int visible = this->VisibleLines();

        for (int row = 0; row < visible; row++) {
            qint64 line = first_line + row;
            if (line >= this->LineCount())
                break;

            if (line % 2) {
                qreal y = row * this->line_height_;
                painter.fillRect(QRectF(0, y, this->viewport()->width(), this->line_height_), this->bg_alt_color_);
            }
        }

        painter.translate(this->content_offset_x_ - this->horizontalScrollBar()->value(), 0);

        for (int row = 0; row < visible; row++) {
            qint64 line = first_line + row;
            if (line >= this->LineCount())
                break;

            qreal y = row * this->line_height_;
            qint64 line_offset = line * this->bytes_per_line_;

            QString addr = QString("%1").arg(line_offset, this->address_width_, 16, QChar('0')).toUpper();
            painter.setPen(this->address_color_);
            painter.drawText(QRectF(this->char_width_, y, this->address_width_ * this->char_width_, this->line_height_),
                              Qt::AlignVCenter | Qt::AlignLeft, addr);

            for (int col = 0; col < this->bytes_per_line_; col++) {
                qint64 offset = line_offset + col;
                if (offset >= this->data_.size())
                    break;

                quint8 b = static_cast<quint8>(this->data_.at(offset));
                bool selected = this->IsSelected(offset);

                qreal hx = this->ByteCellX(col);
                if (selected)
                    painter.fillRect(QRectF(hx, y, this->char_width_ * 2, this->line_height_), this->selection_bg_);

                painter.setPen(this->hex_color_);
                QString hex = QString("%1").arg(b, 2, 16, QChar('0')).toUpper();
                painter.drawText(QRectF(hx, y, this->char_width_ * 2, this->line_height_), Qt::AlignVCenter | Qt::AlignLeft, hex);

                qreal ax = this->AsciiColumnX() + col * this->char_width_;
                if (selected)
                    painter.fillRect(QRectF(ax, y, this->char_width_, this->line_height_), this->selection_bg_);

                QChar c = (b >= 32 && b < 127) ? QChar(static_cast<char>(b)) : QChar('.');
                painter.setPen(this->ascii_color_);
                painter.drawText(QRectF(ax, y, this->char_width_, this->line_height_), Qt::AlignVCenter | Qt::AlignLeft, QString(c));
            }
        }

        painter.setPen(this->separator_color_);
        qreal hex_sep_x = this->HexColumnX() - this->char_width_;
        qreal ascii_sep_x = this->AsciiColumnX() - this->char_width_;
        painter.drawLine(QPointF(hex_sep_x, 0), QPointF(hex_sep_x, this->viewport()->height()));
        painter.drawLine(QPointF(ascii_sep_x, 0), QPointF(ascii_sep_x, this->viewport()->height()));
    }

    void HexViewport::resizeEvent(QResizeEvent* event) {
        QAbstractScrollArea::resizeEvent(event);
        this->RecalculateBytesPerLine();
        this->UpdateScrollRange();
        this->viewport()->update();
    }

    qint64 HexViewport::OffsetFromPoint(const QPoint& pos) const {
        int abs_x = pos.x() - static_cast<int>(this->content_offset_x_) + this->horizontalScrollBar()->value();
        qint64 line = this->verticalScrollBar()->value() + static_cast<qint64>(pos.y() / this->line_height_);

        int col = -1;

        for (int i = 0; i < this->bytes_per_line_; i++) {
            qreal cell_x = this->ByteCellX(i);
            if (abs_x >= cell_x && abs_x < cell_x + this->char_width_ * 2) {
                col = i;
                break;
            }
        }

        if (col == -1) {
            qreal ascii_x = this->AsciiColumnX();
            if (abs_x >= ascii_x) {
                int c = static_cast<int>((abs_x - ascii_x) / this->char_width_);
                if (c >= 0 && c < this->bytes_per_line_)
                    col = c;
            }
        }

        if (col == -1)
            col = 0;

        qint64 offset = line * this->bytes_per_line_ + col;
        return std::clamp<qint64>(offset, 0, std::max<qint64>(0, this->data_.size() - 1));
    }

    void HexViewport::mousePressEvent(QMouseEvent* event) {
        if (event->button() != Qt::LeftButton || this->data_.isEmpty())
            return;

        qint64 offset = this->OffsetFromPoint(event->pos());
        this->selection_start_ = offset;
        this->selection_end_ = offset;
        this->viewport()->update();
    }

    void HexViewport::mouseMoveEvent(QMouseEvent* event) {
        if (!(event->buttons() & Qt::LeftButton) || this->data_.isEmpty())
            return;

        this->selection_end_ = this->OffsetFromPoint(event->pos());
        this->viewport()->update();
    }

    void HexViewport::keyPressEvent(QKeyEvent* event) {
        if (event->matches(QKeySequence::Copy)) {
            this->CopySelectionAsHex();
            event->accept();
            return;
        }

        if (event->matches(QKeySequence::SelectAll)) {
            this->SelectAll();
            event->accept();
            return;
        }

        switch (event->key()) {
            case Qt::Key_Up:
                this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - 1);
                break;
            case Qt::Key_Down:
                this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + 1);
                break;
            case Qt::Key_PageUp:
                this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - this->VisibleLines());
                break;
            case Qt::Key_PageDown:
                this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() + this->VisibleLines());
                break;
            default:
                QAbstractScrollArea::keyPressEvent(event);
                return;
        }
        event->accept();
    }
}
