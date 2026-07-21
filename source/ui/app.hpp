#pragma once

#include <ui/views/binary/view.hpp>
#include <ui/views/hex/view.hpp>

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QStackedWidget>
#include <QWidget>

namespace ui {
    class App : public QMainWindow {
        Q_OBJECT

    public:
        explicit App(int argc, char** argv, QWidget* parent = nullptr);

    private:
        QStackedWidget* view_stack_;
        ui::views::BinaryView* binary_view_;
        ui::views::HexView* hex_view_;

        /* ui */
        void WindowInit();
        void MenuInit();
        void WorkViewsInit();

        /* events */
        void OpenBinaryEvent();
        void CloseEvent(QCloseEvent* event);

    protected:
        void closeEvent(QCloseEvent* event) override { this->CloseEvent(event); }
    };
}
