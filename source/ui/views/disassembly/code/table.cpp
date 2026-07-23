#include <ui/views/disassembly/code/table.hpp>
#include <QScrollBar>
#include <QPainter>
#include <QMouseEvent>
#include <cmath>

namespace ui::views {
    CodeTable::CodeTable(QWidget* parent) : QTableView(parent) {
        this->setMouseTracking(true);
        this->resize_timer_ = new QTimer(this);
        this->connect(this->resize_timer_, &QTimer::timeout, this, &CodeTable::SmoothResize);
    }

    int CodeTable::FindResizeColumn(int x) {
        if (!this->model()) return -1;

        int current = -this->horizontalScrollBar()->value();
        int column_count = this->model()->columnCount();

        /* exclude last column, it should not be drag-resizable */
        for (int i = 0; i < column_count - 1; i++) {
            current += this->columnWidth(i);
            if (std::abs(x - current) <= 4) return i;
        }

        return -1;
    }

    void CodeTable::mouseMoveEvent(QMouseEvent* event) {
        if (this->resizing_) {
            int difference = static_cast<int>(event->position().x()) - this->start_x_;
            int new_width = std::max(this->start_width_ + difference, this->kMinColumnWidth);

            this->target_width_ = new_width;
            if (!this->resize_timer_->isActive())
                this->resize_timer_->start(16);

            this->viewport()->update();
            return;
        }

        int column = this->FindResizeColumn(static_cast<int>(event->position().x()));
        this->viewport()->setCursor(column != -1 ? Qt::SplitHCursor : Qt::ArrowCursor);

        QTableView::mouseMoveEvent(event);
    }

    void CodeTable::mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
            int column = this->FindResizeColumn(static_cast<int>(event->position().x()));
            if (column != -1) {
                this->resizing_ = true;
                this->resize_column_ = column;
                this->start_x_ = static_cast<int>(event->position().x());
                this->start_width_ = this->columnWidth(column);
                return;
            }
        }

        QTableView::mousePressEvent(event);
    }

    void CodeTable::mouseReleaseEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton && this->resizing_) {
            this->resizing_ = false;
            if (this->resize_column_ != -1)
                this->setColumnWidth(this->resize_column_, std::max(this->target_width_, this->kMinColumnWidth));
            this->resize_column_ = -1;
            this->resize_timer_->stop();
        }

        QTableView::mouseReleaseEvent(event);
    }

    void CodeTable::SmoothResize() {
        if (this->resize_column_ == -1) {
            this->resize_timer_->stop();
            return;
        }

        int current = this->columnWidth(this->resize_column_);
        int clamped_target = std::max(this->target_width_, this->kMinColumnWidth);
        int difference = clamped_target - current;

        if (std::abs(difference) <= 1) {
            this->setColumnWidth(this->resize_column_, clamped_target);
            this->resize_timer_->stop();
            return;
        }

        int step = static_cast<int>(difference * 0.25);
        if (step == 0) step = difference > 0 ? 1 : -1;
        this->setColumnWidth(this->resize_column_, current + step);
    }

    void CodeTable::paintEvent(QPaintEvent* event) {
        QTableView::paintEvent(event);
        if (!this->model()) return;

        QPainter painter(this->viewport());
        int offset = this->horizontalScrollBar()->value();
        int cumulative = -offset;

        /* draw one separator line after each column except the last */
        for (int i = 0; i < this->model()->columnCount() - 1; i++) {
            cumulative += this->columnWidth(i);
            if (cumulative >= 0 && cumulative <= this->viewport()->width())
                painter.drawLine(cumulative, 0, cumulative, this->viewport()->height());
        }
    }

    void CodeTable::scrollTo(const QModelIndex& index, ScrollHint hint) {
        int h_scroll = this->horizontalScrollBar()->value();
        QTableView::scrollTo(index, hint);
        this->horizontalScrollBar()->setValue(h_scroll); /* restore original horizontal position */
    }
}
