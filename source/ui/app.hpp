#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>

namespace ui {
    class App : public QMainWindow {
        Q_OBJECT

    public:
        explicit App(int argc, char** argv, QWidget* parent = nullptr);

    private:
        void WindowInit();
        void MenuInit();
        void OpenBinary();

        void closeEvent(QCloseEvent* event) override {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this,
                "Exit byteknife",
                "Are you sure you want to exit?",
                QMessageBox::Yes | QMessageBox::No
            );

            if (reply == QMessageBox::Yes)
                event->accept();
            else
                event->ignore();
        }
    };
}
