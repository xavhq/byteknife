#pragma once

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

        /* ui */
        void WindowInit();
        void MenuInit();
        void WorkViewsInit();

        /* events */
        void OpenBinaryEvent();
        void CloseEvent(QCloseEvent* event);

        void closeEvent(QCloseEvent* event) override { this->CloseEvent(event); }
    };
}
