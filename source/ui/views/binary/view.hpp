#pragma once

#include <QWidget>
#include <QSplitter>

namespace ui::views {
    class BinaryView : public QWidget {
        Q_OBJECT

    public:
        explicit BinaryView(QWidget* parent = nullptr);

    private:
        QSplitter* main_splitter_;
        QWidget* functions_;
        QWidget* disassembly_;
        QWidget* xrefs_;
    };
}
