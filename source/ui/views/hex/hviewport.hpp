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
        void SetAddressWidth(int width);

        void FindPatternFromUser(); /* prompts, then finds first match from start */
        void FindNext();            /* repeats last pattern, from just after current match */
        void RunSearchDirect(const QString& pattern);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void resizeEvent(QResizeEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void keyPressEvent(QKeyEvent* event) override;
        void contextMenuEvent(QContextMenuEvent* event) override;
        void changeEvent(QEvent* event) override;

    private:
        QByteArray data_;
        bool read_only_ = true;
        bool centered_ = true;
        bool auto_fit_ = true;
        int bytes_per_line_ = 16;
        int min_bytes_per_line_ = 8;
        int max_bytes_per_line_ = 32;
        int address_width_ = 8;
        qreal char_width_ = 0;
        qreal line_height_ = 0;
        qreal content_offset_x_ = 0;
        qint64 selection_start_ = -1;
        qint64 selection_end_ = -1;

        QColor bg_color_;
        QColor bg_alt_color_;
        QColor address_color_;
        QColor hex_color_;
        QColor ascii_color_;
        QColor separator_color_;
        QColor selection_bg_;
        QColor unused_color_;

        QString last_pattern_;
        qint64 last_match_offset_ = -1;
        qint64 last_match_length_ = 0;

        void UpdateMetrics();
        void UpdateScrollRange();
        void UpdateColors();
        void RecalculateBytesPerLine();
        qint64 LineCount() const;
        int VisibleLines() const;
        qreal ContentWidth() const;
        qreal HexColumnX() const;
        qreal ByteCellX(int column) const;
        qreal AsciiColumnX() const;
        bool IsSelected(qint64 offset) const;
        qint64 OffsetFromPoint(const QPoint& pos) const;

        void SelectAll();
        void CopySelectionAsHex();
        void CopySelectionAsAscii();
        void CopySelectedAddress();

        /* returns match offset, and writes match length via out param, -1 if not found */
        qint64 FindPattern(const QString& pattern, qint64 start_offset, qint64& out_length);
        void RunSearch(const QString& pattern, qint64 start_offset, bool wrap_message_if_missing);
        bool last_search_mode_was_text_ = false;
    };
}
