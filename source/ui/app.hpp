#pragma once

#include <ui/views/disassembly/view.hpp>
#include <ui/views/hex/view.hpp>
#include <ui/app/components/CommandLineToken.hpp>

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStackedWidget>
#include <QWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QLineEdit>

#include <iostream>

namespace ui {
    class App : public QMainWindow {
        Q_OBJECT

    public:
        explicit App(int argc, char** argv, QWidget* parent = nullptr);

    private:
        QStackedWidget* view_stack_;
        ui::views::DisassemblyView* disassembly_view_;
        QWidget decompiler_view_;
        ui::views::HexView* hex_view_;
        QLineEdit* command_input_;
        QTabWidget* work_view_tabs_;

        /* ui */
        void WindowInit();
        void MenuInit();
        void WorkViewsInit();
        void CommandBarInit();

        /* events */
        void OpenBinaryEvent();
        void CloseEvent(QCloseEvent* event);
        void CommandSendEvent();
        void DispatchCommandEvent(CLTokenVector& Tokens);

        /* settings */
        void SaveTabOrder();
        void RestoreTabOrder();

        /* commands */
        void help();
        void find(CLTokenVector&);
        void goto_f(CLTokenVector&);
        void tab(CLTokenVector&);
    protected:
        void closeEvent(QCloseEvent* event) override { this->CloseEvent(event); }
    };
}
