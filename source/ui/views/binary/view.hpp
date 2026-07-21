#pragma once
#include <ui/app/components/ViewPlaceHolderContent.hpp>
#include <ui/views/binary/disasm/dwidget.hpp>

#include <QWidget>
#include <QSplitter>
#include <QTableWidget>
#include <QStackedWidget>

namespace ui::views {
    class BinaryView : public QWidget {
        Q_OBJECT
    public:
        explicit BinaryView(QWidget* parent = nullptr);

        void AddInstructionRow(const QString& address, const QString& bytes, const QString& text);
        void ClearDisassembly();

        /* events */
        void OnBinaryOpen();
        void OnBinaryClosed();

    private:
        /* ui */
        QSplitter* main_splitter_;
        QStackedWidget* disassembly_stack_;
        DisassemblyWidget* disassembly_;
        QWidget* functions_;
        QWidget* xrefs_;

        ui::views::ViewPlaceHolderContent* placeholder_;
    };
}
