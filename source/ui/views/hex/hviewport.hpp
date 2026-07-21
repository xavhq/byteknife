#pragma once
#include <QAbstractScrollArea>
#include <QByteArray>
#include <QColor>

namespace ui::views {
    class HexViewport : public QAbstractScrollArea {
        Q_OBJECT
    public:
        explicit HexViewport(QWidget* parent = nullptr);

        void SetData(const QByteArray& data);
        void Clear();
        QByteArray SelectedBytes() const;
        void SetReadOnly(bool read_only);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        QByteArray data_;
        bool read_only_ = true;
        bool centered_ = true; /* center content block when widget is wider than content */

        int bytes_per_line_ = 16;
        int address_width_ = 8;

        qreal char_width_ = 0;
        qreal line_height_ = 0;
        qreal content_offset_x_ = 0; /* horizontal offset when centered */

        qint64 selection_start_ = -1;
        qint64 selection_end_ = -1;

        /* colors are pulled from the current QPalette, cached here on theme change */
        QColor bg_color_;
        QColor bg_alt_color_;
        QColor address_color_;
        QColor hex_color_;
        QColor ascii_color_;
        QColor separator_color_;
        QColor selection_bg_;

        void UpdateMetrics();
        void UpdateScrollRange();
        void UpdateColors();
        qint64 LineCount() const;
        int VisibleLines() const;
        qreal ContentWidth() const;

        qreal HexColumnX() const;
        qreal ByteCellX(int column) const;
        qreal AsciiColumnX() const;

        bool IsSelected(qint64 offset) const;
        qint64 OffsetFromPoint(const QPoint& pos) const;
    };
}
