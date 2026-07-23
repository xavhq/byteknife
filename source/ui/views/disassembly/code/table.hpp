#pragma once
#include <QTableView>
#include <QTimer>

namespace ui::views {
    class CodeTable : public QTableView {
        Q_OBJECT
    public:
        explicit CodeTable(QWidget* parent = nullptr);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible) override;

    private:
        int resize_column_ = -1;
        bool resizing_ = false;
        int start_x_ = 0;
        int start_width_ = 0;
        int target_width_ = 0;
        static constexpr int kMinColumnWidth = 50;

        QTimer* resize_timer_;

        int FindResizeColumn(int x);
        void SmoothResize();
    };
}
