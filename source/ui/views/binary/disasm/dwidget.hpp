#pragma once
#include <QWidget>
#include <ui/views/binary/disasm/dtable.hpp>
#include <ui/views/binary/disasm/dmodel.hpp>

namespace ui::views {
    class DisassemblyWidget : public QWidget {
        Q_OBJECT
    public:
        explicit DisassemblyWidget(QWidget* parent = nullptr);

        void AddInstructionRow(const QString& address, const QString& bytes, const QString& text);
        void ClearDisassembly();

    private:
        DisassemblyTable* table_;
        DisassemblyModel* model_;
    };
}
