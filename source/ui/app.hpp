#pragma once

#include <ui/views/disassembly/view.hpp>
#include <ui/views/hex/view.hpp>
#include <ui/views/output/view.hpp>
#include <ui/app/components/CommandLineToken.hpp>

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStackedWidget>
#include <QWidget>
#include <QLineEdit>
#include <QTabWidget>
#include <QLineEdit>
#include <QCloseEvent>

#include <iostream>
#include <functional>

namespace ui {
    inline std::function<void()> OnOpenBinaryRequested;

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
        ui::views::Output* output_;
        QDockWidget* output_dock_;

        /* ui */
        void WindowInit();
        void MenuInit();
        void WorkViewsInit();
        void CommandBarInit();
        void OutputInit();

        /* events */
        void OpenBinaryEvent();
        void CloseEvent(QCloseEvent*);
        void CommandSendEvent();
        void DispatchCommandEvent(CLTokenVector&);

        /* settings */
        void SaveTabOrder();
        void RestoreTabOrder();
        void BindCheckableSetting(QAction* action, const QString& key, bool default_value, std::function<void(bool)> apply);

        /* commands */
        void help();
        void find(CLTokenVector&);
        void goto_f(CLTokenVector&);
        void tab(CLTokenVector&);
        void openbin() { this->OpenBinaryEvent(); }
        void exit() { this->close(); }

    protected:
        void closeEvent(QCloseEvent* event) override { this->CloseEvent(event); }
        bool eventFilter(QObject*, QEvent*) override;
    };
}
