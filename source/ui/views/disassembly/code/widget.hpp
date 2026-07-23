#pragma once
#include <QWidget>
#include <ui/views/disassembly/code/table.hpp>
#include <ui/views/disassembly/code/model.hpp>

namespace ui::views {
    class CodeWidget : public QWidget {
        Q_OBJECT
    public:
        explicit CodeWidget(QWidget* parent = nullptr);

        void AddInstructionRow(const QString& address, const QString& bytes, const QString& text);
        void ClearDisassembly();

    private:
        CodeTable* table_;
        CodeModel* model_;
    };
}
