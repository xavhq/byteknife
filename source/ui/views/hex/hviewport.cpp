#include <ui/views/hex/hviewport.hpp>

#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFontMetricsF>
#include <QEvent>

namespace ui::views {
    HexViewport::HexViewport(QWidget* parent) : QAbstractScrollArea(parent) {
        this->setFocusPolicy(Qt::StrongFocus);
        this->viewport()->setCursor(Qt::IBeamCursor);
        this->UpdateColors();
        this->UpdateMetrics();
    }

    void HexViewport::SetReadOnly(bool read_only) {
        this->read_only_ = read_only; /* reserved for future editing support */
    }

    void HexViewport::UpdateColors() {
        const QPalette& pal = this->palette();

        /* base surface colors, following the current color scheme */
        this->bg_color_ = pal.color(QPalette::Base);
        this->bg_alt_color_ = pal.color(QPalette::AlternateBase);

        /* text colors, dimmed variant for addresses via disabled-state text */
        this->address_color_ = pal.color(QPalette::Disabled, QPalette::Text);
        this->hex_color_ = pal.color(QPalette::Text);
        this->ascii_color_ = pal.color(QPalette::Text);

        this->separator_color_ = pal.color(QPalette::Mid);

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

        /* pick an address width wide enough for the data, minimum 8 hex digits */
        this->address_width_ = (this->data_.size() > 0xFFFFFFFFLL) ? 16 : 8;

        this->UpdateMetrics();
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

    void HexViewport::UpdateScrollRange() {
        qint64 lines = this->LineCount();
        int visible = this->VisibleLines();

        this->verticalScrollBar()->setRange(0, static_cast<int>(std::max<qint64>(0, lines - visible)));
        this->verticalScrollBar()->setPageStep(std::max(1, visible - 1));

        qreal content_width = this->ContentWidth();
        int viewport_width = this->viewport()->width();

        if (this->centered_ && content_width < viewport_width) {
            /* content fits, center it and disable horizontal scrolling */
            this->content_offset_x_ = (viewport_width - content_width) / 2.0;
            this->horizontalScrollBar()->setRange(0, 0);
        } else {
            this->content_offset_x_ = 0;
            int overflow = static_cast<int>(content_width - viewport_width);
            this->horizontalScrollBar()->setRange(0, std::max(0, overflow));
        }
    }

    qreal HexViewport::HexColumnX() const {
        /* 1 char left margin + address digits + 2 char gap */
        return (1 + this->address_width_ + 2) * this->char_width_;
    }

    qreal HexViewport::ByteCellX(int column) const {
        qreal x = this->HexColumnX();
        for (int i = 0; i < column; i++) {
            x += this->char_width_ * 3; /* "FF " */
            if ((i + 1) % 8 == 0)
                x += this->char_width_; /* extra gap every 8 bytes */
        }
        return x;
    }

    qreal HexViewport::AsciiColumnX() const {
        qreal x = this->ByteCellX(this->bytes_per_line_ - 1) + this->char_width_ * 3;
        x += this->char_width_ * 2; /* gap before ascii column */
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

        /* draw alternating row stripes across the FULL viewport width,
           before translating, so they always reach both edges regardless
           of content centering or horizontal scroll */
        for (int row = 0; row < visible; row++) {
            qint64 line = first_line + row;
            if (line >= this->LineCount())
                break;

            if (line % 2) {
                qreal y = row * this->line_height_;
                painter.fillRect(QRectF(0, y, this->viewport()->width(), this->line_height_), this->bg_alt_color_);
            }
        }

        /* now translate for the actual content (address/hex/ascii columns) */
        painter.translate(this->content_offset_x_ - this->horizontalScrollBar()->value(), 0);

        for (int row = 0; row < visible; row++) {
            qint64 line = first_line + row;
            if (line >= this->LineCount())
                break;

            qreal y = row * this->line_height_;
            qint64 line_offset = line * this->bytes_per_line_;

            /* address column */
            QString addr = QString("%1").arg(line_offset, this->address_width_, 16, QChar('0')).toUpper();
            painter.setPen(this->address_color_);
            painter.drawText(QRectF(this->char_width_, y, this->address_width_ * this->char_width_, this->line_height_),
                              Qt::AlignVCenter | Qt::AlignLeft, addr);

            /* hex + ascii columns */
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

        /* column separators */
        painter.setPen(this->separator_color_);
        qreal hex_sep_x = this->HexColumnX() - this->char_width_;
        qreal ascii_sep_x = this->AsciiColumnX() - this->char_width_;
        painter.drawLine(QPointF(hex_sep_x, 0), QPointF(hex_sep_x, this->viewport()->height()));
        painter.drawLine(QPointF(ascii_sep_x, 0), QPointF(ascii_sep_x, this->viewport()->height()));
    }

    void HexViewport::resizeEvent(QResizeEvent* event) {
        QAbstractScrollArea::resizeEvent(event);
        this->UpdateScrollRange();
    }

    qint64 HexViewport::OffsetFromPoint(const QPoint& pos) const {
        int abs_x = pos.x() - static_cast<int>(this->content_offset_x_) + this->horizontalScrollBar()->value();
        qint64 line = this->verticalScrollBar()->value() + static_cast<qint64>(pos.y() / this->line_height_);

        int col = -1;

        /* check hex region first */
        for (int i = 0; i < this->bytes_per_line_; i++) {
            qreal cell_x = this->ByteCellX(i);
            if (abs_x >= cell_x && abs_x < cell_x + this->char_width_ * 2) {
                col = i;
                break;
            }
        }

        /* fall back to ascii region */
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
