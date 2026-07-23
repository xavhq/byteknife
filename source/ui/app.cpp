#include <ui/app.hpp>

#include <QApplication>
#include <QLabel>

namespace ui {
    App::App(int argc, char** argv, QWidget* parent) : QMainWindow(parent) {
        /* TODO: byteknife binary (opening binaries directly) */
        (void)argc;
        (void)argv;

        this->WindowInit();
        this->MenuInit();
        this->WorkViewsInit();
        this->CommandBarInit();
    }
}
